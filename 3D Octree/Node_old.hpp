#ifndef NODE_OLD_H_
#define NODE_OLD_H_

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

    // * double quadrupole;

public:
    
    // * each node can have up to 8 children = octants
    // * name: quadtree/2D convention + up/down in z-direction
    Node* NW_up;
    Node* NW_down;
    Node* NE_up;
    Node* NE_down;
    Node* SW_up;
    Node* SW_down;
    Node* SE_up;
    Node* SE_down;

public:


    // * Needed to initialise the root
    Node() {

        this->barycentre = Vec3d();
        this->N = 0;
        this->mass = 0.;
        this->centre = Vec3d();
        this->size = 1.;

        // * initially, node has no children
        this->NW_up = NULL;
        this->NW_down = NULL;
        this->NE_up = NULL;
        this->NE_down = NULL;
        this->SW_up = NULL;
        this->SW_down = NULL;
        this->SE_up = NULL;
        this->SE_down = NULL;

    };

    Node(Particle elem, Vec3d centre, double size) {

        this->barycentre = elem.r();
        this->mass = elem.m();
        this->N = 1;
        this->centre = centre;
        this->size = size;

        this->NW_up = NULL;
        this->NW_down = NULL;
        this->NE_up = NULL;
        this->NE_down = NULL;
        this->SW_up = NULL;
        this->SW_down = NULL;
        this->SE_up = NULL;
        this->SE_down = NULL;
    };

    ~Node() {
        delete this->NW_up;
        delete this->NW_down;
        delete this->NE_up;
        delete this->NE_down;
        delete this->SW_up;
        delete this->SW_down;
        delete this->SE_up;
        delete this->SE_down;
    };

public:

    // * Getters to extract the data
    Vec3d bary() { return barycentre; };
    double N() { return N; };
    double m() { return mass; };
    double s() { return size; };

    // * Update the total mass and barycentre when a particle is added
    void update(Particle elem) {
        double new_mass = mass + elem.m();
        Vec3d new_barycentre = (mass * barycentre + elem.m() * elem.r()) / new_mass;
        this->mass = new_mass;
        this->barycentre = new_barycentre;
        this->N++;
    };

    // * Function which returns octant given a particle
    // * based on subdivision in octants
    string getOctant(Particle elem) {

        Vec3d comparison = elem.r();
        if(comparison.x() > this->centre.x()) {
            if(comparison.y() > this->centre.y()) {
                if(comparison.z() > this->centre.z()) {
                    return "NE_up";
                }
                else {
                    return "NE_down";
                }
            }
            else {
                if(comparison.z() > this->centre.z()) {
                    return "SE_up";
                }
                else {
                    return "SE_down";
                }
            }
        }
        else {
            if(comparison.y() > this->centre.y()) {
                if(comparison.z() > this->centre.z()) {
                    return "NW_up";
                }
                else {
                    return "NW_down";
                }
            }
            else {
                if(comparison.z() > this->centre.z()) {
                    return "SW_up";
                }
                else {
                    return "SW_down";
                }   
            }
        }
    };

    // * Calculate the new geometrical centre for a subnode using the one of the parent
    // * Dependent on the octant, we have to change how the vector is added
    Vec3d calcSubCentre(string octant) {
        if(octant == "NE_up") {
            return this->centre + (this->size / 2.) * Vec3d(1., 1., 1.);
        }
        else if(octant == "NE_down") {
            return this->centre + (this->size / 2.) * Vec3d(1., 1., -1.);
        }
        else if(octant == "SE_up") {
            return this->centre + (this->size / 2.) * Vec3d(1., -1., 1.);
        }
        else if(octant == "SE_down") {
            return this->centre + (this->size / 2.) * Vec3d(1., -1., -1.);
        }
        else if(octant == "NW_up") {
            return this->centre + (this->size / 2.) * Vec3d(-1., 1., 1.);
        }
        else if(octant == "NW_down") {
            return this->centre + (this->size / 2.) * Vec3d(-1., 1., -1.);
        }
        else if(octant == "SW_up") {
            return this->centre + (this->size / 2.) * Vec3d(-1., -1., 1.);
        }
        else {
            return this->centre + (this->size / 2.) * Vec3d(-1., -1., -1.);
        }
    };


    // * we need get to the subnode dependent on the octant
    Node** getSubnode(string octant) {
        if(octant == "NE_up") {
            return &NE_up;
        }
        else if(octant == "NE_down") {
            return &NE_down;
        }
        else if(octant == "SE_up") {
            return &SE_up;
        }
        else if(octant == "SE_down") {
            return &SE_down;
        }
        else if(octant == "NW_up") {
            return &NW_up;
        }
        else if(octant == "NW_down") {
            return &NW_down;
        }
        else if(octant == "SW_up") {
            return &SW_up;
        }
        else {
            return &SW_down;
        }
    };

    // * Insertion recursive method requires 3 cases
    // * if there is no particle in the node, create that node with the particle
    // * if there is 1 particle: get octant of both and make childnodes dependent on octant
    // * if there is > 1 particle: get octant and insert in childnode, other particles already done

    void insert(Particle elem) {
            
        // * more than 1 particle present
        if(this->N > 1) {

            string octant = getOctant(elem);
            Node** node_octant = getSubnode(octant);

            if(!(*node_octant)) {
                Vec3d centre_subnode = calcSubCentre(octant);
                *node_octant = new Node(elem, centre_subnode, this->size / 2.);
            }
            else {
                (*node_octant)->insert(elem);
            }

        }

        // * already 1 particle present
        else if(this->N == 1) {

            Particle particle_old = Particle(this->barycentre, Vec3d(), this->mass);
            string octant_old = getOctant(particle_old);
            Node** node_octant_old = getSubnode(octant_old);

            if(!(*node_octant_old)) {
                Vec3d centre_subnode = calcSubCentre(octant_old);
                *node_octant_old = new Node(elem, centre_subnode, this->size / 2.);
            }
            else {
                (*node_octant_old)->insert(elem);
            }

            string octant_new = getOctant(elem);
            Node** node_octant_new = getSubnode(octant_new);

            if(!(*node_octant_new)) {
                Vec3d centre_subnode = calcSubCentre(octant_new);
                *node_octant_new = new Node(elem, centre_subnode , this->size / 2.);
            }
            else {
                (*node_octant_new)->insert(elem);
            }

        }

        // * Must always happen and accounts for N=0
        this->update(elem); 

    }
    
    // * Print the tree in postorder
    void postorder(Node* node) {
        if(node != NULL) {
            cout << this->N << " " << endl;
            postorder(this->NE_up);
            postorder(this->NE_down);
            postorder(this->SE_up);
            postorder(this->SE_down);
            postorder(this->NW_up);
            postorder(this->NW_down);
            postorder(this->SW_up);
            postorder(this->SW_down);
        }
    }

};


#endif