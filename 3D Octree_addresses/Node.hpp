#ifndef NODE_H_
#define NODE_H_

#include<set>
#include "Particle.hpp"

// * building block of the tree, tree itself is a big node with a lot of chidren, grandchildren etc
// * This class will define each node having some parameters and children

class Node {
private:

    unsigned int N; // * amount of particles
    Vec3d barycentre;
    double mass;
    // * Geometrical parameters
    Vec3d centre;
    double size;

    // * vector of the particle adresses, important to traverse the node and make sure it gets
    // * to the particle leaf and its siblings without restrictions on delta
    // * also needed for quadrupole moment
    set<Particle*> addresses;
    // * double quadrupole;

public: 
    // * each node can have up to 8 children = octants
    // * name: quadtree/2D convention + up/down in z-direction
    // * index denotes which quadrant, can be written as binary zyx (see sorting)
    // * order: SW_down - SE_down - NW_down - NE_down - SW_up - SE_up - NW_up - NE_up 
    Node* octants[8];
   

public:

    // * Needed to initialise the root
    Node(double size) {

        this->barycentre = Vec3d();
        this->N = 0;
        this->mass = 0.;
        this->centre = Vec3d();
        this->size = size;
        
        // * initially, node has no children
        for(unsigned int i = 0; i < 8; i++) {
            this->octants[i] = nullptr;
        }
    };

    Node(Particle* elem, Vec3d centre, double size) {

        this->barycentre = elem->r();
        this->mass = elem->m();
        this->N = 1;
        this->centre = centre;
        this->size = size;
        this->addresses = {elem};

        // * initially, node has no children
        for(unsigned int i = 0; i < 8; i++) {
            this->octants[i] = nullptr;
        }
    };

    ~Node() {
        for(unsigned int i = 0; i < 8; i++) {
            delete this->octants[i];
        }

    };

public:

    // * Getters to extract the data
    Vec3d bary() { return barycentre; };
    unsigned int amount() { return N; };
    double m() { return mass; };
    double s() { return size; };
    Node* octant(unsigned int i) { return octants[i]; }


    // * Update the total mass and barycentre when a particle is added
    void update(Particle* elem) {
        double new_mass = mass + elem->m();
        Vec3d new_barycentre = (this->mass * this->barycentre + elem->m() * elem->r()) / new_mass;
        this->mass = new_mass;
        this->barycentre = new_barycentre;
        this->N++;
        this->addresses.insert(elem);
    };

    // * Function which returns octant given a particle
    // * based on subdivision in octants
    // * using binary formaty zyx where x_i > x_centre means 1
    // * returns index of corresponding octant
    unsigned int getOctant(Vec3d position) {
        unsigned int i = 0;
        if(position.x() > this->centre.x()) { i++; }
        if(position.y() > this->centre.y()) { i += 2; }
        if(position.z() > this->centre.z()) { i += 4; }
        return i; 
    };
    
    // * Calculate the new geometrical centre for a subnode using the one of the parent
    // * Dependent on the octant, we have to change how the vector is added, based on sign last vector
    Vec3d calcSubCentre(unsigned int i) {
        double x = pow(-1, i + 1);
        double y = pow(-1, (i / 2) + 1);
        double z = pow(-1, (i / 4) + 1);
        return this->centre + (this->size / 4.) * Vec3d(x, y, z);
    };

    // * Insertion recursive method requires 3 cases
    // * if there is no particle in the node, create that node with the particle
    // * if there is 1 particle: get octant of both and make childnodes dependent on octant
    // * if there is > 1 particle: get octant and insert in childnode, other particles already done

    void insert(Particle* elem) {
        // * more than 1 particle present
        if(this->N > 1) {

            unsigned int octant = getOctant(elem->r());

            if(!this->octants[octant]) {
                Vec3d centre_subnode = calcSubCentre(octant);
                this->octants[octant] = new Node(elem, centre_subnode, this->size / 2.);
            }
            else {
                this->octants[octant]->insert(elem);
            }

        }

        // * already 1 particle present
        else if(this->N == 1) {

            unsigned int octant_old = getOctant(this->barycentre);

            if(!this->octants[octant_old]) {
                Vec3d centre_subnode = calcSubCentre(octant_old);
                // * Since the set in this case only contains 1 element, the first one is the old particle
                this->octants[octant_old] = new Node(*this->addresses.begin(), centre_subnode, this->size / 2.);
            }
            else {
                this->octants[octant_old]->insert(*this->addresses.begin());
            }

            unsigned int octant_new = getOctant(elem->r());

            if(!this->octants[octant_new]) {
                Vec3d centre_subnode = calcSubCentre(octant_new);
                this->octants[octant_new] = new Node(elem, centre_subnode , this->size / 2.);
            }
            else {
                this->octants[octant_new]->insert(elem);
            }
        }

        // * Must always happen
        this->update(elem); 

    }

    // * Function which builds the tree
    void buildTree(vector<Particle>& particles) {
    for (Particle &elem : particles) {
        this->insert(&elem);
        }
    }

    // * Force exerted by the node/leave on a particle with softening if needed
    Vec3d force(Vec3d relative_vector, double eps, bool softening) {
        if(softening) {
            return (-1) * (this->mass * relative_vector) / (pow( hypot(eps, relative_vector.norm()), 3 ));
        }
        else {
            return (-1) * (this->mass * relative_vector) / relative_vector.norm3();
        }
    }

    // * Calculate the total force of a particle with softening if needed
    Vec3d totalForce(Particle* elem, double eps, double delta) {
        Vec3d force = Vec3d();

        // * Make sure the traversal is at least through its parents!
        if(this->N > 1 and this->addresses.find(elem) != this->addresses.end()) {
            for(unsigned int i = 0; i < 8; i++)  {
                if(this->octants[i]) {
                    force += this->octants[i]->totalForce(elem, eps, delta);
                }
            }
        } 
        else {

            Vec3d relative_vector = elem->r() - this->barycentre;

            // * N = 1 means this is a leaf = particle
            if(this->N == 1) {
                // * Explicitly check if it's not the particle itself!
                if(this->addresses.find(elem) == this->addresses.end()) {
                    force = this->force(relative_vector, eps, true);
                }

            }
            else {
                // * Check for the condition
                if( (this->size / relative_vector.norm()) < delta ) {
                    // * treat as conglomerate
                    force = this->force(relative_vector, eps, true);
                }
                else {
                    // * no leaf and not deep enough -> do the same for all subnodes
                    for( unsigned int i = 0; i < 8; i++ ) {
                        if(this->octants[i]) {
                            force += this->octants[i]->totalForce(elem, eps, delta);
                        }
                    }
                }

            }
        }

        return force;

    }

    // * Print the tree in postorder
    void postorder() {
        if(this) {
            cout << this->m() << " " << endl;
            for(unsigned int i = 0; i < 8; i++) {
                this->octants[i]->postorder();
            }
        }
    }   
};


#endif
