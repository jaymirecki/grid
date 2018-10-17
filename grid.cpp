#include "grid.h"

using namespace std;

Grid::Grid(int uheight, int uwidth) {
        height = uheight;
        width = uwidth;

        details = new cell**[height];
        closed = new bool*[height];
        obstacle = new bool*[height];
        visual = new char*[height];
        for (int i = 0; i < height; i++) {
                details[i] = new cell*[width];
                closed[i] = new bool[width];
                obstacle[i] = new bool[width];
                visual[i] = new char[width];
                for (int j = 0; j < width; j++) {
                        details[i][j] = new cell;
                        details[i][j]->location = coord (j, i);
                        obstacle[i][j] = false;
                        visual[i][j] = VALID;
                }

        }
}
Grid::~Grid() {
        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    delete details[i][j];
                }
                delete [] details[i];
                delete [] closed[i];
                delete [] obstacle[i];
                delete [] visual[i];
        }
                delete [] details;
                delete [] closed;
                delete [] obstacle;
                delete [] visual;
}
int Grid::distance(coord origin, coord destination) {
        for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                        closed[i][j] = false;

        empty_details();

        open.insert(cPair (0, get_details(origin)));

        while (!open.empty()) {
                cell *cur= open.begin()->second;
                open.erase(open.begin());
                closed[cur->location.second]
                      [cur->location.first] = true;

                if (add_neighbors(cur, destination) != -1)
                        return add_neighbors(cur, destination);
        }
        return -1;
}
list<coord> Grid::shortest_path(coord origin, coord destination) {
        distance(origin, destination);
        list<coord> path;
        return find_path(destination, path);
}
void Grid::print_path_list(list<coord> path) {
        for (list<coord>::iterator it = path.begin(); it != path.end(); it++)
                cout << "(" << get_details(*it)->location.first << ", " 
                     << get_details(*it)->location.second << ") ";
        cout << endl;
}
void Grid::print_path_grid(list<coord> path) {
        for (list<coord>::iterator it = path.begin(); it != path.end(); it++)
                visual[get_details(*it)->location.second][get_details(*it)->location.first] = PATH;

        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++)
                        cout << visual[i][j] << " ";
                cout << endl;
        }

        for (list<coord>::iterator it = path.begin(); it != path.end(); it++)
                visual[get_details(*it)->location.second][get_details(*it)->location.first] = VALID;
}
void Grid::print_grid() {
        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++)
                        cout << visual[i][j] << " ";
                cout << endl;
        }
}


void Grid::empty_details() {
        for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++) {
                        details[i][j]->parent = coord (0, 0);
                        details[i][j]->f = -1;
                        details[i][j]->g = -1;
                        details[i][j]->h = -1;
                }
}
Grid::cell *Grid::get_details(coord location) {
        return details[location.second][location.first];
}
int Grid::add_neighbors(cell *parent, coord destination) {
        int newg = parent->g + 1;

        for (int i = 0; i < 4; i++) {
                cell *next;
                coord next_coord = neighbor(parent->location, i);
                if (valid(next_coord)) {
                        next = get_details(next_coord);
                
                        if (next->location == destination) {
                                next->parent = parent->location;
                                return newg;
                        }

                        if (next->g > newg || next->g == -1) {
                                next->g = newg;
                                next->h = h(next->location, 
                                                   destination);
                                next->f = newg + next->h;
                                next->parent = parent->location;
                                open.insert(cPair (next->f, next));
                        }
                }
        }

        return -1;
}
coord Grid::neighbor(coord location, int i) {
        switch (i) {
                case 0:
                        return coord (location.first - 1, location.second);
                case 1:
                        return coord (location.first, location.second - 1);
                case 2:
                        return coord (location.first + 1, location.second);
                case 3:
                        return coord (location.first, location.second + 1);
                default:
                        return location;
        }
}
bool Grid::valid(coord location) {
        return (location.first < width && location.first >= 0 
                && location.second < height && location.second >= 0) 
               && !obstacle[location.second][location.first]
               && !closed[location.second][location.first];
}
double Grid::h(coord location, coord destination) {
        return sqrt(pow(abs(location.first - destination.first), 2) 
                    + pow(abs(location.second - destination.second), 2));
}
list<coord> Grid::find_path(coord destination, list<coord> &path) {
        cell *cur = get_details(destination);
        path.push_front(destination);
        if (cur->location == cur->parent)
                return path;
        return find_path(cur->parent, path);
}
void Grid::block(coord location) {
        obstacle[location.second][location.first] = true;
        visual[location.second][location.first] = INVALID;
}
void Grid::block(list<coord> locations) {
        for (list<coord>::iterator it = locations.begin(); it != locations.end(); it++)
                block(*it);
}