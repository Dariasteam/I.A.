#include "agent.hpp"

Agent::Agent(int x, int y):
    x_(x),
    y_(y),
    noFinded_(true),
    possibleEnd_(NULL),
    paths_()
  {
    QList<Pos> aux;
    aux.push_back({x,y});
    paths_.push_back({aux,0});
}


Agent::~Agent() {}


void Agent::insertSort(Path path) {
    if (paths_.empty()) {
        paths_.push_back(path);
    }
    else {
        auto it = paths_.begin();
        bool inserted = false;
        while (it != paths_.end() && !inserted) {
            if (it->cost_ > path.cost_) {
                paths_.insert(it,path);
                inserted = true;
            }
            it++;
        }
        if (!inserted)
            paths_.push_back(path);
    }
}

Pos Agent::operator()(QList<CellWeight> tr) {
    if (!joinPath_.path_.empty()) {
        return joinPath_.path_.takeLast();
    }
    cout << "Nuevo camino"<< endl;
    for (auto i:paths_) {
        cout << "cost : "<< i.cost_ << " {";
        for (auto j :i.path_) {
            cout << "(x: " << j.x_ << " y:" << j.y_ << ")";
        }
        cout << endl;
    }
    if (noFinded_) {
        Path path = paths_.front();
        paths_.pop_front();
        QList<CellWeight> auxTr;
        for (auto it = tr.begin(); it != tr.end(); it++) {
            bool isIn = false;
            for (auto i: path.path_) {

                if (((it->x_ == i.x_) && (it->y_ ==i.y_))) {
                    isIn = true;
                }
            }
            if (!isIn)
                auxTr.push_back(*it);
        }

        for (auto it = auxTr.begin(); it != auxTr.end(); it++) {
            Path aux = path;
            aux.path_.push_back({it->x_, it->y_});
            aux.cost_ += it->weight_;
            insertSort(aux);
        }
        noFinded_ = !paths_.empty();
        if(noFinded_) {
            pathJoin(paths_.front(), path);
            x_ = paths_.front().path_.back().x_;
            y_ = paths_.front().path_.back().y_;
        }
    }
    return {x_,y_};
}

void Agent::pathJoin(Path p_, Path p2_) {
    cout << "PathJoin" << endl;
    cout << "p_ : " << " {";
    for (auto j :p_.path_) {
        cout << "(x: " << j.x_ << " y:" << j.y_ << ")";
    }
    cout << endl;
    cout << "p2_" << " {";
    for (auto j :p2_.path_) {
        cout << "(x:" << j.x_ << " y:" << j.y_ << ")";
    }
    cout << endl;
    list<Pos> p = p_.path_.toStdList();
    list<Pos> p2 = p2_.path_.toStdList();
    bool stop = false;
    auto i = p2.rbegin();
    while (i != p2.rend() && !stop) {
        Path aux;
        auto j = p.rbegin();
        while (j != p.rend() && !stop) {
            if ((i->x_ == j->x_) && (i->y_ == j->y_)) {
                stop = true;
            } else {
                aux.path_.push_front(*j);
            }
            j++;
        }
        joinPath_.path_.push_back(*i);
        if (stop) {
            joinPath_.path_ += aux.path_;
            break;
        }
        i++;
    }
    cout << "joinPath" << " {";
    for (auto j :joinPath_.path_) {
        cout << "(x:" << j.x_ << " y:" << j.y_ << ")";
    }
    cout << endl;
}
