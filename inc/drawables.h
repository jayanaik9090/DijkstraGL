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
#include <functional> //greater
using namespace std;
class Node;
typedef struct COORD_ {
    SHORT x;
    SHORT y;
    SHORT z;
} coord;

typedef pair<Node*,Node*> Edge;
typedef map<Node*,set<Node*>> NeighbourMap;
typedef map<Edge,pair<COLOR,float>> EdgeColorCostMap;
typedef map<Node*,Node*> NodeNodeMap;
typedef vector<Node*> PathVector;
typedef pair<int,Node*> intNodePair;

int WIDTH=25;


class Drawable {
public:
    Drawable() {}

    virtual ~Drawable() {}

    virtual void draw() {}
};


class Text : public Drawable{

private:
    string s;
    int x,y;
public:
    Text(string,int x,int y);    
    void draw(void* font=GLUT_BITMAP_9_BY_15);
};






class Node : public Drawable {

private:
    
    bool highlight;
    RECT bound;
    COLOR hcolor = {0,1,0};

public:
    static int count;
    float r, g, b;
    int wt;
    bool selected;
    coord pos;
    string id;
    Node(coord pos, float r,float g,float b);
    void setHighlight(bool,COLOR hcolor={0,1,0});
    void setColor(float r,float g,float b);
    void draw();
    bool collides(int,int); 
    void Node::setPos(int x,int y);
};


Node::Node(coord pos, float r, float g, float b)
{   //cout<<"Node created!\n";
    this->pos = pos;
    this->r = r;
    this->g = g;
    this->b = b;
    id = string(1,'a'+count++); //create new string on single character
    highlight = false;
    selected = false;
    wt=-1;
    bound = {pos.x - WIDTH, pos.y - WIDTH, pos.x + WIDTH, pos.y + WIDTH};
}


void Node::draw() {
    Drawable::draw(); //call base class draw method
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin(GL_QUADS);
        //0.8,0,0.3
        //glColor3f(0.8, 0, 0.3);
        glColor3ub(r,g,b);
        COLOR green = {255,0,0};
        COLOR yel = {255,255,0};
        if(highlight && hcolor == green)
            glColor3ub(0,255,0);
        if(highlight && hcolor == yel)
            glColor3ub(255,0,0);

        glVertex3f(bound.left,bound.bottom,1);
        glVertex3f(bound.right,bound.bottom,1);
        glVertex3f(bound.right,bound.top,1);    
        glVertex3f(bound.left,bound.top,1);

    glEnd();

    if (highlight && hcolor!=green && hcolor!=yel){
        glLineWidth(2);
        glColor3f(hcolor[0],hcolor[1],hcolor[2]);
        glBegin(GL_LINE_LOOP);
            glVertex3f(bound.left-15,bound.bottom+15,-1);
            glVertex3f(bound.right+15,bound.bottom+15,-1);
            glVertex3f(bound.right+15,bound.top-15,-1);
            glVertex3f(bound.left-15,bound.top-15,-1);
        glEnd();
    }
    int lw=2.0;
    glBegin(GL_POLYGON);
        glColor3ub(0,0,0);
        if(highlight && hcolor==green)
        glColor3ub(0,100,0);
        glVertex3f(bound.left-lw,bound.bottom+lw,-1);
        glVertex3f(bound.right+lw,bound.bottom+lw,-1);
        glVertex3f(bound.right+lw,bound.top-lw,-1);    
        glVertex3f(bound.left-lw,bound.top-lw,-1);


    glEnd();

    glPopMatrix();
    glColor3f(1,1,1);
    Text t (id,pos.x,pos.y);
    t.draw();

    glColor3f(1,1,1);
        glRectf(pos.x-15,pos.y-28,pos.x+15,pos.y-43);
    
    glColor3f(0,0,0);
    char buf[100];
    sprintf(buf, "%d", wt);
    string s(buf);
    if(wt==-1)
        s="inf";
    Text t1 (s,pos.x,pos.y-35);
    t1.draw();

}


void Node::setHighlight(bool val,COLOR hcolor={0,1,0}) {
    highlight = val;
    this->hcolor = hcolor;
  //  cout<<"Node "<<id<<" highlight with value: "<<this->highlight<< endl;
}

void Node::setColor(float r,float g,float b){
    this->r=r;
    this->g=g;
    this->b=b;

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
    {   //cout<<"Hit: "<<id<<endl;
        highlight=true;
        return true;
    }
    highlight=false;
    return false;

}


Text::Text(string s,int x,int y){
    //cout<<"String: "<<s<<endl;
    this->s=s;
    this->x=x;
    this->y=y;
}

void Text::draw(void* font) {
        // cout<<cstr<<endl;
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
    static map<Node*,Node*> parent;
    static PathVector pathVec;
    static void addEdge(Edge);
    static void removeEdge(Node* u);
    static void setEdgeColor(Edge edge,COLOR c);
    static int dijkstra(list<Node> &v,Node* src,Node *dest);
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
    edgeColors[edge].second = rand()%10+1; //seting cost
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
        coord pos1 = u.first->pos;
        coord pos2 = v->pos;
        Edge edge = {u.first,v};
        const COLOR c = edgeColors[edge].first;
        const COLOR r = {255,0,0};
        const COLOR g = {0,255,0};
        const COLOR y = {255,255,0};
        glLineWidth(2.0);
        if(c==r || c==g ||c==y)
            glLineWidth(8.0);
        glBegin(GL_LINES);
            

            glColor3ub(c[0],c[1],c[2]);
            if(c==r)
                glColor3ub(0,255,0);
            glVertex3f(pos1.x,pos1.y,-1.5);
            glVertex3f(pos2.x,pos2.y,-1.5);


        glEnd();
        coord mid = {(pos1.x+pos2.x)/2,(pos1.y+pos2.y)/2};
        glColor3f(1,1,1);
        glRectf(mid.x-15,mid.y+15,mid.x+15,mid.y-15);
        glColor3f(0,0,0);

        int cost = edgeColors[edge].second;
        Text t (to_string(cost),mid.x,mid.y+2);
        if(c!=r)
            t.draw(GLUT_BITMAP_HELVETICA_18);  
        else
            t.draw(GLUT_BITMAP_TIMES_ROMAN_24);
    }   
}



void Graph::path(Node *v){
    if(parent[v]==NULL)
    {
        //cout<<v->id<<",";
        pathVec.push_back(v);
        return;
    }

    path(parent[v]);
   // cout<<v->id<<",";
    pathVec.push_back(v);
}

static void Graph::reset(){
    for(auto &i:edgeColors)
        i.second.first = {0,0,0};
    pathVec.clear();
    parent.clear(); 
}
#endif //DIJKSTRA_DRAWABLES_H
