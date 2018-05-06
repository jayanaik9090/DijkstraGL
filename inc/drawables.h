//
// Created by ::arvind:: on 4/30/2018.
//

#ifndef DIJKSTRA_DRAWABLES_H
#define DIJKSTRA_DRAWABLES_H
#define inf 0x3f3f3f3f
#include <iostream>
#include <utility>
#include <map>
#include <set>
#include <GL/glut.h>
#include "color.h"
#include <queue>
#include <functional> //std::greater
class Node;
typedef struct COORD_ {
    SHORT x;
    SHORT y;
    SHORT z;
} coord;

typedef std::pair<Node*,Node*> Edge;
typedef std::map<Node*,std::set<Node*>> NeighbourMap;
typedef std::map<Edge,std::pair<COLOR,float>> EdgeColorCostMap;
typedef std::map<Node*,Node*> NodeNodeMap;
typedef std::vector<Node*> PathVector;
int WIDTH=20;


class Drawable {
public:
    Drawable() {}

    virtual ~Drawable() {}

    virtual void draw() {}
};


class Text : public Drawable{

private:
    std::string s;
    int x,y;
public:
    Text(std::string,int x,int y);    
    void draw(void* font=GLUT_BITMAP_9_BY_15);
};






class Node : public Drawable {

private:
    
    bool highlight;
    RECT bound;

public:
    static int count;
    float r, g, b;
    bool selected;
    coord pos;
    std::string id;
    Node(coord pos, float r,float g,float b);
    void setHighlight(bool);
    void draw();
    bool collides(int,int); 
    void Node::setPos(int x,int y);
};


Node::Node(coord pos, float r, float g, float b)
{   std::cout<<"Node created!\n";
    this->pos = pos;
    this->r = r;
    this->g = g;
    this->b = b;
    id = std::string(1,'a'+count++); //create new string on single character
    highlight = false;
    selected = false;
    bound = {pos.x - WIDTH, pos.y - WIDTH, pos.x + WIDTH, pos.y + WIDTH};
}


void Node::draw() {
    Drawable::draw(); //call base class draw method
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin(GL_QUADS);
        glColor3ub(r, g, b);
        //glColor3ub(105,105,105);
        glVertex3f(bound.left,bound.bottom,1);
        glVertex3f(bound.right,bound.bottom,1);
        glVertex3f(bound.right,bound.top,1);    
        glVertex3f(bound.left,bound.top,1);

    glEnd();

    if (highlight){
        glLineWidth(2);
        glColor3fv(color::GREEN);
        glBegin(GL_LINE_LOOP);
            glVertex3f(bound.left-15,bound.bottom+15,-1);
            glVertex3f(bound.right+15,bound.bottom+15,-1);
            glVertex3f(bound.right+15,bound.top-15,-1);
            glVertex3f(bound.left-15,bound.top-15,-1);
        glEnd();
    }

    glPopMatrix();

    glColor3f(1,1,1);
    Text t (id,pos.x,pos.y);
    t.draw();
}


void Node::setHighlight(bool val) {
    highlight = val;
  //  std::cout<<"Node "<<id<<" highlight with value: "<<this->highlight<< std::endl;
}

void Node::setPos(int x,int y){
    pos = {x,y};
    bound = {pos.x - WIDTH, pos.y - WIDTH, pos.x + WIDTH, pos.y + WIDTH};
}

bool Node::collides(int x,int y){
    if( x >= bound.left-20 
    &&  x <= bound.right+20  
    &&  y >= bound.top-20
    &&  y <= bound.bottom+20)
    {   //std::cout<<"Hit: "<<id<<std::endl;
        highlight=true;
        return true;
    }
    highlight=false;
    return false;

}


Text::Text(std::string s,int x,int y){
    //std::cout<<"String: "<<s<<std::endl;
    this->s=s;
    this->x=x;
    this->y=y;
}

void Text::draw(void* font) {
        // std::cout<<cstr<<std::endl;
         const char *cstr=s.c_str();
        glPushMatrix();
        int w = glutBitmapLength(font, cstr);

        glRasterPos3f(x - (float)w / 2, y + 4,1.5);
        
        for (char* c = cstr; *c != '\0'; c++) {

            glutBitmapCharacter(font, *c);
        }
        glPopMatrix();
}

int Node::count=0;


class Graph{
public:    
    static NeighbourMap neighbours;
    static EdgeColorCostMap edgeColors;
    static std::map<Node*,Node*> parent;
    static PathVector pathVec;
    static void addEdge(Edge);
    static void removeEdge(Node* u);
    static void setEdgeColor(Edge edge,COLOR c);
    static int dijkstra(std::list<Node> &v,Node* src,Node *dest);
    static void draw();
    static void path(Node *v);
    static void Graph::reset();

};

NeighbourMap Graph::neighbours {}; //initialize map 
EdgeColorCostMap Graph::edgeColors {};
NodeNodeMap Graph::parent {};
PathVector Graph::pathVec {};

static void Graph::setEdgeColor(Edge edge,COLOR c){
    edgeColors[edge].first = c; 
    edgeColors[{edge.second,edge.first}].first = c;    
}

static void Graph::addEdge(Edge edge){
    neighbours[edge.first].insert(edge.second);
    neighbours[edge.second].insert(edge.first);
    COLOR c = {0,0,0};//{edge.first->r,edge.first->g,edge.first->b};
    setEdgeColor(edge,c);
    std::cout<<"Input Cost for edge ("<<edge.first->id<<","<<edge.second->id<<") : ";
    std::cin>>edgeColors[edge].second;
    Edge edge2 = {edge.second,edge.first};
    edgeColors[edge2].second=edgeColors[edge].second;
}

static void Graph::removeEdge(Node* u){
     for(auto &s:neighbours[u])
        neighbours[s].erase(u);
    neighbours.erase(u);
}

static void Graph::draw(){
    
  //  glColor3fv(colors[u,v]);
    for(auto &u:neighbours)
    for(auto &v:u.second){
        glBegin(GL_LINES);
        coord pos1 = u.first->pos;
        coord pos2 = v->pos;
        Edge edge = {u.first,v};
        const COLOR c = edgeColors[edge].first;
        glColor3ub(c[0],c[1],c[2]);
        glVertex3f(pos1.x,pos1.y,-1.5);
        glVertex3f(pos2.x,pos2.y,-1.5); 
         glEnd();
        coord mid = {(pos1.x+pos2.x)/2,(pos1.y+pos2.y)/2};
        glColor3f(1,1,1);
        glRectf(mid.x-15,mid.y+15,mid.x+15,mid.y-15);
        glColor3f(0,0,0);
        int cost = edgeColors[edge].second;
        Text t (std::to_string(cost),mid.x,mid.y+2);
        t.draw(GLUT_BITMAP_HELVETICA_18);  
    }   
}

typedef std::pair<int,Node*> intNodePair;
static int Graph::dijkstra(std::list<Node> &v,Node* src,Node *dest){
    std::priority_queue <intNodePair,std::vector<intNodePair>,std::greater<intNodePair>> pq;
    std::map<Node*,int> dist;//(v.size(),inf);
    for(auto &i:v)
    {   dist[&i]=inf;
        parent[&i]=NULL;
    }
    pq.push({0,src});
    dist[src]=0;
    while(!pq.empty()){
        Node* u = pq.top().second;
        pq.pop();
        for(auto &i:neighbours[u]){
            Node *v = i;
            Edge e = {u,v};
            int wt=edgeColors[e].second;
            if(dist[u]+wt < dist[v])
            {
                dist[v] = dist[u]+wt;
                pq.push({dist[v],v});
                parent[v]=u;
            }
        }
    }

    // std::cout<<"vertex\t\tdistance\tpath"<<std::endl;
    // for(auto &i:dist){
    //     std::cout<<i.first->id<<"\t\t"<<dist[i.first]<<"\t\t";
    //     if(dist[i.first]!=inf) path(i.first);
    //    std::cout<<std::endl;
    // }

    path(dest);
    for(auto i=0;i<pathVec.size()-1;i++)
        setEdgeColor({pathVec[i],pathVec[i+1]},{0,255,0});//
    return dist[dest];
  //      edgeColors[{pathVec[i],pathVec[i+1]}].first= {0,255,0};

}

void Graph::path(Node *v){
    if(parent[v]==NULL)
    {
        std::cout<<v->id<<",";
        pathVec.push_back(v);
        return;
    }

    path(parent[v]);
    std::cout<<v->id<<",";
    pathVec.push_back(v);
}

static void Graph::reset(){
    for(auto &i:edgeColors)
        i.second.first = {0,0,0};
    pathVec.clear();
    parent.clear(); 
}
#endif //DIJKSTRA_DRAWABLES_H
