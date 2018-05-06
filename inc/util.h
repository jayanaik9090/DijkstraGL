// Created by ghostship on 5/1/2018.
//

#ifndef DIJKSTRA_UTIL_H
#define DIJKSTRA_UTIL_H


#include "glcontext.h"
#define FONT1 GLUT_BITMAP_9_BY_15

namespace util{

    std::list<Node> v;
    Edge curEdge = {NULL,NULL};;
    float n = 20 ;//sort count
    float lineWidth = 4;
    bool animate = true;
    bool start=false;
    Node* curNode = NULL;
    Node* source = NULL;
    Node* dest = NULL;
    int getNode(int x,int y);
    void draw_string(char*, int, int, void* = FONT1);
    void initRandomLines();
    void addMessage(std::string s,int index);
    void show_message(int x, int y,std::string s,void* font=GLUT_BITMAP_TIMES_ROMAN_24);
    std::vector<std::string> msgs(5,""); 

    void keyboard(unsigned char key, int x, int y)
    {

        if(key ==27)
            exit(0);

        if(key == 'n')
        {
            for(auto &i:Graph::neighbours){
                std::cout<<"neighbours["<<i.first->id<<"] :";
                for(auto j:i.second)
                    std::cout<<j->id<<",";
            std::cout<<std::endl;
            }
            std::cout<<std::endl;
            std::cout<<std::endl;
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
    {   
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
        //GLContext::redraw();
    }

    


    void mouse(int btn,int state,int x,int y)
    {
        if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
            start=true;
            
            if(!curNode)
            {
                v.push_back(Node({x,y,-1},rand()%255,rand()%255,rand()%255));
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
            {    curEdge = {NULL,NULL}; std::cout<<"Reseting\n";}
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
     //    std::cout<<"("<<x<<","<<y<<") "<<std::endl;

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
        //    std::cout<<"("<<x<<","<<y<<") "<<curEdge.first->id<<std::endl;
        }
    }



    //helper functions
    void defaultMenuHandler(int option){
        if(option==3)
            {v.clear();msgs.clear(); Node::count=0;Graph::neighbours.clear();Graph::edgeColors.clear();Graph::parent.clear();Graph::pathVec.clear();}
        if(option==2){
            if(source==NULL)
            {addMessage("src: No source selected!",0);return;}
            if(dest==NULL)
            {addMessage("dest: No dest selected!",1);return;}    
            int cost = Graph::dijkstra(v,source,dest);
            addMessage("cost: "+std::to_string(cost),2);

        }

        if(option==1){
            Graph::reset();
            msgs[0]="src: ?";
            msgs[1]="dest:?";
            msgs[2]="cost:?";
        }
    }

    void addMessage(std::string s,int index){
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

    



    void show_message(int x, int y,std::string s,void* font) {
        
        glColor3f(0,0,1);
        Text t(s,x,y);
        t.draw(font);
    }

    

    

}

#endif //DIJKSTRA_UTIL_H
