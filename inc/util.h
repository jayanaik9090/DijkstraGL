// Created by ghostship on 5/1/2018.
//

#ifndef DIJKSTRA_UTIL_H
#define DIJKSTRA_UTIL_H


#include "glcontext.h"
//#include <windows.h>
#define FONT1 GLUT_BITMAP_9_BY_15
namespace util{

    list<Node> v;
    Edge curEdge = {NULL,NULL};;
    float n = 20 ;//sort count
    float lineWidth = 4;
    bool animate = true;
    bool start=false;
    bool busy = false;
    Node* curNode = NULL;
    Node* source = NULL;
    Node* dest = NULL;
    int getNode(int x,int y);
    void draw_string(char*, int, int, void* = FONT1);
    void initRandomLines();
    void addMessage(string s,int index);
    void show_message(int x, int y,string s,void* font=GLUT_BITMAP_TIMES_ROMAN_24);
    void clearScreen();
    vector<string> msgs(5,""); 

    void keyboard(unsigned char key, int x, int y)
    {

        if(key ==27)
            exit(0);

        if(key == 'n')
        {
            for(auto &i:Graph::neighbours){
                cout<<"neighbours["<<i.first->id<<"] :";
                for(auto j:i.second)
                    cout<<j->id<<",";
            cout<<endl;
            }
            cout<<endl;
            cout<<endl;
        }

        if(key == 43) // +
        { WIDTH++; for(auto &i:v) i.setPos(i.pos.x,i.pos.y);}
        if(key == 45) // -
        { WIDTH--; for(auto &i:v) i.setPos(i.pos.x,i.pos.y);}


        GLContext::post(); //redisplay

    }

    static void reshape(int w,int h){
        float aspect = (float)w / (float)h;
        glViewport(0, 0, w, h);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, w, h,0, -2, 2);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }


    void display()
    {   //cout<<"display\n";
        GLContext::clear();
        GLContext::drawNodes(v);
        GLContext::drawEdges();

        int w = glutGet(GLUT_WINDOW_WIDTH);
        int x = w -150;
        if(!start){
            show_message(w/2,500/2,"Click to add a new node");
            show_message(w/2,70,"A Project by: Ipsita & [vin]");
            show_message(w/2,40,"Visualing Dijkstra's Algortihm");
        }
        if(!msgs.empty())
        for(int i=0;i<5;i++)
            {show_message(x,30+30*i,msgs[i],GLUT_BITMAP_9_BY_15);}
        GLContext::update();
    }


    void idle()
    {
       // GLContext::post();
    }

    


    void mouse(int btn,int state,int x,int y)
    {   
        

        if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
            start=true;
            
            if(!curNode)
            {   //0.8, 0, 0.3
                v.push_back(Node({x,y,-1},0.8*255,0*255,0.3*255));
            }
            else if(curNode->selected )
            { 
                curNode->selected=false; //if node selected for moving, when clicked again, drop it.
            }
            else //node present below mouse, it gets selected
            {
                curEdge.first=curNode;
                glutSetCursor(GLUT_CURSOR_CROSSHAIR);
            }
        }

        if(btn==GLUT_LEFT_BUTTON && state==GLUT_UP){
            for(auto &i:v)
                if(i.collides(x,y)){
                    curNode=&i;
                }

            if(!curNode || curEdge.first == curNode)
            {    curEdge = {NULL,NULL}; //cout<<"Reseting\n";
            }
            else if(curNode!=curEdge.first && curEdge.first!=NULL)
            {
                curEdge.second=curNode;
                Graph::addEdge(curEdge);

                curEdge = {NULL,NULL};
            }

            glutSetCursor(GLUT_CURSOR_INHERIT);    //on click up, reset cursor
        }

        GLContext::post();


    }

    
    void nodeMenu();
    void defaultMenu();
    

    void motion(int x,int y){
        
        bool collides = false;
        
        for(auto &i:v)
            if(i.collides(x,y)){
                collides=true;
                curNode=&i;
            }

        if(collides)
            nodeMenu();
        else{
            defaultMenu(); 
            if(curNode && !curNode->selected) curNode=NULL;
        }
        
       if(curNode && curNode->selected)
         curNode->setPos(x-10,y-10);  
        //if(curEdge.first)
     //    cout<<"("<<x<<","<<y<<") "<<endl;

        GLContext::post();
    }

    void mouseMotion(int x,int y){

        //for dotted line
        if(curEdge.first)   {
            GLContext::clear();
            glPushAttrib(GL_ENABLE_BIT);
            glLineStipple(1,0X1111);
            glEnable(GL_LINE_STIPPLE);
            glColor3f(0,0,0);
            glBegin(GL_LINES);
            glVertex3f(curEdge.first->pos.x,curEdge.first->pos.y,1);
            glVertex3f(x,y,1);
            glEnd();
            glPopAttrib();
            GLContext::drawNodes(v);
            GLContext::drawEdges();
            GLContext::update();
        //    cout<<"("<<x<<","<<y<<") "<<curEdge.first->id<<endl;
        }
    }



    //helper functions
    void defaultMenuHandler(int option){
        if(option==2){
            if(source==NULL)
            {addMessage("src: No source selected!",0);return;}
            if(dest==NULL)
            {addMessage("dest: No dest selected!",1);return;}
            busy=true;    
            int cost = Graph::dijkstra(v,source,dest)
;            busy=false;
            addMessage("cost: "+to_string(cost),2);

        }

        if(option==1){
            Graph::reset();
            msgs[0]="src: ?";
            msgs[1]="dest:?";
            msgs[2]="cost:?";
        }

        if(option==3)
            {clearScreen();}
        
    }

    void addMessage(string s,int index){
        msgs[index]=s;
    }

    void nodeMenuHandler(int option){
        switch(option){
            case 1: addMessage("src: node "+curNode->id,0); source=curNode;break;
            case 2: addMessage("dest: node "+curNode->id,1);dest=curNode;break;
            case 3: Graph::removeEdge(curNode);
                    for(auto n = v.begin(); n!=v.end();n++)
                    if(&*n == curNode) {v.erase(n);return;}
            case 4: curNode->selected = true; 

        }
    }


    void nodeMenu(){
        glutCreateMenu(nodeMenuHandler);
        glutAddMenuEntry("Set as start node",1);
        glutAddMenuEntry("Set as destination node",2);
        glutAddMenuEntry("Delete node",3);
        glutAddMenuEntry("Move node",4);

        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }

    void defaultMenu(){
        glutCreateMenu(defaultMenuHandler);
        glutAddMenuEntry("Reset Graph",1);
        glutAddMenuEntry("Find shortest path",2);
        glutAddMenuEntry("Clear",3);
        
        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }

    



    void show_message(int x, int y,string s,void* font) {
        
        glColor3f(0,0,1);
        Text t(s,x,y);
        t.draw(font);
    }

    void redraw(){
        GLContext::clear();
        GLContext::drawNodes(util::v);
        GLContext::drawEdges();
        GLContext::update();
    } 

static int Graph::dijkstra(list<Node> &v,Node* src,Node *dest){
    priority_queue <intNodePair,vector<intNodePair>,greater<intNodePair>> pq;
    map<Node*,int> dist;//(v.size(),inf);
    map<Node*,bool> visited;

    for(auto &i:v)
    {   dist[&i]=inf;
        parent[&i]=NULL;
        visited[&i]=false;   
    }

    pq.push({0,src});
    dist[src]=0;
    src->wt=0;
    while(!pq.empty()){
        Node* u = pq.top().second;
        cout<<"Pop: "<<u->id<<", "<<pq.top().first<<endl;
        pq.pop();
        
        if(visited[u]){
            
            Sleep(500);
            util::redraw();
            continue;
        }

        visited[u] = true;
        u->setHighlight(true,{255,0,0});

        util::redraw();
        Sleep(500);
        for(auto &i:neighbours[u]){
            Node *v = i;
            Edge e = {u,v};
            setEdgeColor(e,{255,0,0});
            v->setHighlight(true,{255,255,0});
            util::redraw();
            Sleep(650);;
            //util::redraw();

            int wt=edgeColors[e].second;
            if(dist[u]+wt < dist[v] && !visited[v])
            {
                dist[v] = dist[u]+wt;
                pq.push({dist[v],v});
                v->wt=dist[v];
                cout<<"pushing: "<<v->id<<", "<<dist[v]<<endl;
                parent[v]=u;
            }
            setEdgeColor(e,{0,0,0});
            v->setHighlight(false);
            util::redraw();
            Sleep(500);
            util::redraw();
        }

        Sleep(500);

        u->setHighlight(false);
        u->setColor(0.2,0.2,0.2); //grey
    }

    // cout<<"vertex\t\tdistance\tpath"<<endl;
    // for(auto &i:dist){
    //     cout<<i.first->id<<"\t\t"<<dist[i.first]<<"\t\t";
    //     if(dist[i.first]!=inf) path(i.first);
    //    cout<<endl;
    // }


    path(dest);
    for(auto i=0;i<pathVec.size()-1;i++)
    {   
        setEdgeColor({pathVec[i],pathVec[i+1]},{255,255,0});//
        util::redraw();
        Sleep(100);
    }

    pathVec[0]->setColor(0,0,255);
    pathVec.back()->setColor(0,255,0);
    GLContext::post();
    return dist[dest];
 
}

void clearScreen(){
    v.clear();
    Node::count=0;
    Graph::neighbours.clear();
    Graph::edgeColors.clear();
    Graph::parent.clear();
    Graph::pathVec.clear();

}
    

}

#endif //DIJKSTRA_UTIL_H
