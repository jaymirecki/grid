#ifndef GRID_H
#define GRID_H

#include <set>
#include <cmath>
#include <list>
#include <iostream>

typedef std::pair<int, int> coord;

class Grid {

public:
        Grid(int, int);
        ~Grid();
        int distance(coord, coord);
        std::list<coord> shortest_path(coord, coord);
        void print_path_list(std::list<coord>);
        void print_path_grid(std::list<coord>);
        void print_grid();

private:
        struct cell {
                coord parent;
                coord location;
                int g;
                double f, h;
        };
        typedef std::pair<double, cell*> cPair;
        cell ***details;
        bool **closed;
        bool **obstacle;
        std::set<cPair> open;
        char **visual;
        int height, width;
        const char VALID = '.';
        const char INVALID = 'X';
        const char PATH = '@';

        void empty_details();
        cell *get_details(coord);
        int add_neighbors(cell *parent, coord destination);
        coord neighbor(coord location, int i);
        bool valid(coord location);
        double h(coord location, coord destination);
        std::list<coord> find_path(coord, std::list<coord>&);
        void block(coord);
        void block(std::list<coord>);
};

#endif