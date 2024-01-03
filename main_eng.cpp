#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <string.h>
using namespace std;


class V{

    public:
    int id;
    vector <int> successors;
    vector <int> predecessors;

};
class graph{

    public:
    int id;
    int liczba_w;
    vector <V> vtx;
    bool adjoint = true;
    bool line_graph;

};
class orig_graph_edge{

    public:
    V corresponding_adj_vtx;
    int alpha_v = -1;
    int omega_v = -1;

};
class original_graph{

    public:
    graph adjoint_graph;
    vector <orig_graph_edge> edges;
    vector <V> vtx_orig;

};


void graph_loading();
void create_predecessors_list();
bool multigraph_chk(int);
void adjoint_chk(int g);
void line_chk(int g);
bool successors_chk(int, int, int);
bool predecessors_chk(int, int, int);
void create_original_graph(int);
int chk_vtx_orig_ALPHA(original_graph, int);
int chk_vtx_orig_OMEGA(original_graph, int);
void save();


vector <graph> graphs;
vector <original_graph> orig_graph;


int main()
{
    graph_loading();
    create_predecessors_list();
    for(int a = 0; a < graphs.size(); a++){
        multigraph_chk(a);
        if(multigraph_chk(a) == true){
            cout<<"graph "<<a+1<<" NON-ADJOINT: MULTIGRAPH"<<endl;
            break;
            //return?
        }
        adjoint_chk(a);
        if (graphs[a].adjoint == false){
            cout<<"graph "<<a+1<<" NON-ADJOINT"<<endl;
            break;
            //return?
        }
        else if(graphs[a].adjoint == true){
            cout<<"graph "<<a+1<<" ADJOINT"<<endl;
            line_chk(a);
            if(graphs[a].line_graph == true){
                cout<<"graph "<<a+1<<" LINE"<<endl;
            }
            else if(graphs[a].line_graph == false){
                cout<<"graph "<<a+1<<" NON-LINE"<<endl;
            }
            create_original_graph(a);
        }
    }
	
    for(int b = 0; b < orig_graph.size(); b++){
        cout<<"ORIGINAL GRAPH OF ADJOINT GRAPH"<<orig_graph[b].adjoint_graph.id+1<<endl;
        cout<<"LIST OF EDGES AND CORRESPONDING VERTICES OF ADJOINT GRAPH:"<<endl;
        for(int c = 0; c < orig_graph[b].edges.size(); c++){
        cout<<orig_graph[b].edges[c].corresponding_adj_vtx.id<<": "<<orig_graph[b].edges[c].alpha_v<<" "<<orig_graph[b].edges[c].omega_v<<endl;
        }
        cout<<"LIST OF SUCCESSORS:"<<endl;
        for(int e = 0; e < orig_graph[b].vtx_orig.size(); e++){
            cout<<orig_graph[b].vtx_orig[e].id<<": ";
            for(int f = 0; f < orig_graph[b].vtx_orig[e].successors.size(); f++){
                cout<<orig_graph[b].vtx_orig[e].successors[f]<<", ";
                }
            cout<<endl;
        }
    }
    save();

    return 0;
}


void graph_loading(){
    int i = 0;
    ifstream input_file;
    string filename;
	cout<<"Enter input filename: ";
	getline(cin, filename);
    input_file.open(filename.c_str());
    string file_line;
    string temp;
    while(getline(input_file, file_line)){
        cout<<file_line<<endl;
        int v = -1;
        if(file_line == "<adj_list>"){
           graph g;
           g.id = i;
           graphs.push_back(g);
           i++;
            continue;
        }
        else if(file_line != "</adj_list>" && file_line != "<adj_list>"){
            int j = 0;
            int tp = -1;
            while(file_line[j] != ':'){
                temp = file_line.substr(j,1);
                if (tp != -1){
                    tp *= 10;
                    cout<<temp<<endl;
                    tp += stoi(temp);
                    j++;
                }
                else{
                    cout<<temp<<endl;
                    tp = stoi(temp);
                    j++;
                }
            }//eof
            V vertex;
            vertex.id = tp;
            graphs.back().vtx.push_back(vertex);
            j+=1;
            for(j;j<(file_line.length());j++){
                if(file_line[j] == ','){
                    if(v != -1){
                        graphs.back().vtx.back().successors.push_back(v);
                    }
                    v = -1;
                    continue;
                }
                else if(file_line[j] == '/0'){
                    break;
                }
                else{
                    temp = file_line.substr(j,1);
                    if (v != -1){
                        v *= 10;
                        cout<<temp<<endl;
                        v += stoi(temp);
                    }
                    else{
                        cout<<temp<<endl;
                        v = stoi(temp);
                    }
                }
            }//end for
            if(v != -1){
                graphs.back().vtx.back().successors.push_back(v);
            }
        }//end if
        else if(file_line == "</adj_list>"){
            continue;
        }
    }//end while
}


void create_predecessors_list(){

    for(int i = 0; i < graphs.size(); i++){
        for(int j = 0; j < graphs[i].vtx.size(); j++){
            for(int k = 0; k < graphs[i].vtx[j].successors.size(); k++){
                int predecessor = graphs[i].vtx[j].id;
                int preceded = graphs[i].vtx[j].successors[k];
                graphs[i].vtx[preceded].predecessors.push_back(predecessor);
            }
        }
    }
}


bool multigraph_chk(int g){
    vector <int> multi;

    for (int i = 0; i < graphs[g].vtx.size(); i++){
        for(int j = 0; j < graphs[g].vtx[i].successors.size(); j++){
            for(int k = 0; k < multi.size(); k++){
                if(graphs[g].vtx[i].successors[j] == multi[k]){
                    return true;
                }
            }
            multi.push_back(graphs[g].vtx[i].successors[j]);
        }
        multi.clear();
    }
    return false;

}


void adjoint_chk(int g){
    int chker = 0;

    for (int i = 0; i < graphs[g].vtx.size(); i++){
        for(int j = i+1; j < graphs[g].vtx.size(); j++){
                if(j == i)
                    {continue;}
            //compairing pairs of successors
            for(int k = 0; k < graphs[g].vtx[i].successors.size(); k++){
                for(int l = 0; l < graphs[g].vtx[j].successors.size(); l++){
                        if(graphs[g].vtx[i].successors[k] == graphs[g].vtx[j].successors[l]){
                            chker++;
                        }
                }//end for4
            }//end for3
            if(chker != 0 && chker != graphs[g].vtx[i].successors.size() && chker != graphs[g].vtx[j].successors.size()){
                    graphs[g].adjoint = false;
                    graphs[g].line_graph = false;
            }
            chker = 0;
        }//end for2
    }//end for1
}


void line_chk(int g){
    int flag = 0;
	//checking if vertices have the same successors
    for (int i = 0; i < graphs[g].vtx.size(); i++){
        for(int j = i+1; j < graphs[g].vtx.size(); j++){
              if(j == i)
                {continue;}
              else{
                if (successors_chk(g,i,j) == true && predecessors_chk(g,i,j) == true){

                    flag = 1;
                    graphs[g].line_graph = false;
                }

              }//end else
        }//end for2
    }//end for1

    if(flag == 0){
        graphs[g].line_graph = true;
    }
}


bool successors_chk(int g, int i, int j){

    for(int a = 0; a < graphs[g].vtx[i].successors.size(); a++){
            for(int b = 0; b < graphs[g].vtx[j].successors.size(); b++){
                if(graphs[g].vtx[i].successors[a] == graphs[g].vtx[j].successors[b])
                    return true;
            }
    }
    return false;
}


bool predecessors_chk(int g, int i, int j){


    for(int a = 0; a < graphs[g].vtx[i].predecessors.size(); a++){
            for(int b = 0; b < graphs[g].vtx[j].predecessors.size(); b++){
                if(graphs[g].vtx[i].predecessors[a] == graphs[g].vtx[j].predecessors[b])
                    return true;
            }
    }
    return false;
}


void create_original_graph(int g){
    original_graph go;
    go.adjoint_graph = graphs[g];
    int counter = 0;
    for(int i = 0; i < graphs[g].vtx.size(); i++){
        orig_graph_edge e;
        e.corresponding_adj_vtx = graphs[g].vtx[i];
        e.alpha_v = counter;
        counter++;
        e.omega_v = counter;
        counter++;
        go.edges.push_back(e);
    }
    vector <int> wykorzystane;
    int flag = 0;

    for(int j = 0; j < graphs[g].vtx.size(); j++){
        for(int e = 0; e < graphs[g].vtx[j].successors.size(); e++){
            int predecessor = graphs[g].vtx[j].id;
            int successor = graphs[g].vtx[j].successors[e];

            for(int m = 0; m < wykorzystane.size(); m++){
                if(graphs[g].vtx[j].successors[e] == wykorzystane[m]){
                    flag = 1;
                }
            }

            if (flag == 0){
                go.edges[successor].alpha_v = go.edges[predecessor].omega_v;
                wykorzystane.push_back(successor);
            }
            else if(flag == 1){
                go.edges[predecessor].omega_v = go.edges[successor].alpha_v;
                flag = 0;
            }
        }

    }

    for(int ii = 0; ii < go.edges.size(); ii++){

        int chk = chk_vtx_orig_ALPHA(go, ii);
        if(chk == -1){
            V vtxa;
            vtxa.id = go.edges[ii].alpha_v;
            vtxa.successors.push_back(go.edges[ii].omega_v);
            go.vtx_orig.push_back(vtxa);
        }
        else{
            go.vtx_orig[chk].successors.push_back(go.edges[ii].omega_v);
        }
        chk = chk_vtx_orig_OMEGA(go, ii);
        if(chk == -1){
            V vtxo;
            vtxo.id = go.edges[ii].omega_v;
            vtxo.predecessors.push_back(go.edges[ii].alpha_v);
            go.vtx_orig.push_back(vtxo);
        }
        else{
            go.vtx_orig[chk].predecessors.push_back(go.edges[ii].alpha_v);
        }

    }

    int old[go.vtx_orig.size()];

    for(int iii = 0; iii < go.vtx_orig.size(); iii++){
        old[iii] = go.vtx_orig[iii].id;
        go.vtx_orig[iii].id = iii;
    }
    for(int jj = 0; jj < go.vtx_orig.size(); jj++){
        for(int jjj = 0; jjj < go.vtx_orig[jj].successors.size(); jjj++){
            for(int jjjj = 0; jjjj < go.vtx_orig.size(); jjjj++){
                if(go.vtx_orig[jj].successors[jjj] == old[jjjj]){
                    go.vtx_orig[jj].successors[jjj] = jjjj;
                }

            }
        }
    }

    for(int e = 0; e < go.edges.size(); e++){
        for(int ee = 0; ee < go.vtx_orig.size(); ee++){
            if(go.edges[e].alpha_v == old[ee]){
                go.edges[e].alpha_v = ee;
            }
            if(go.edges[e].omega_v == old[ee]){
                go.edges[e].omega_v = ee;
            }
        }
    }


    orig_graph.push_back(go);
}
int chk_vtx_orig_ALPHA(original_graph go, int ii){

    for(int iii = 0; iii < go.vtx_orig.size(); iii++){
            int checked = go.edges[ii].alpha_v;
            int existing = go.vtx_orig[iii].id;
            if(checked == existing){
                return iii;
            }
    }
    return -1;
}
int chk_vtx_orig_OMEGA(original_graph go, int ii){

    for(int iii = 0; iii < go.vtx_orig.size(); iii++){
            int checked = go.edges[ii].omega_v;
            int existing = go.vtx_orig[iii].id;
            if(checked == existing){
                return iii;
            }
    }
    return -1;
}
void save(){

	ofstream out_file;
	string filename;
	cout<<"Enter output filename: ";
	getline(cin, filename);
	out_file.open(filename.c_str());

	for(int i = 0; i < orig_graph.size(); i++){
        out_file<<"<adj_list>"<<endl;
	    for(int j = 0; j < orig_graph[i].vtx_orig.size(); j++){
            string vtx = to_string(orig_graph[i].vtx_orig[j].id);
            out_file<<vtx<<":";
            for(int k = 0; k < orig_graph[i].vtx_orig[j].successors.size(); k++){
                string nx = to_string(orig_graph[i].vtx_orig[j].successors[k]);
                out_file<<nx<<",";
            }
            out_file<<endl;
        }
        out_file<<"</adj_list>"<<endl;

	}

	out_file.close();
}


