#include<bits/stdc++.h>
using namespace std;

#define MAXI 1e8


static bool cmp(const string &a, const string &b) {
    string temp_a = a; temp_a.erase(temp_a.begin());
    string temp_b = b; temp_b.erase(temp_b.begin());
    return stoll(temp_a) < stoll(temp_b);
}

static bool cmp2(const string &a, const string &b) {
    string temp_a = a; temp_a.erase(temp_a.begin());
    string temp_b = b; temp_b.erase(temp_b.begin());
    return stoll(temp_a) > stoll(temp_b);
}





vector<vector<string>> netlist;
vector<vector<string>> in_out_fanout;
vector<vector<string>> ff_list;
vector<vector<string>> gate_list;
vector<vector<string>> fanout;
vector<string>input_list;   // only combi PIs
vector<string>output_wires;
vector<string>wire_list;
vector<string>gate_input_list;

vector<string>ff_d;
vector<string>ff_q;

map<string,int>c_controllability;
map<string,int>s_controllability;



vector<int> ff_c_controllability(string clk, string d, string q) {
    int q0 = c_controllability["CC0_" + clk] + c_controllability["CC1_" + clk] + c_controllability["CC0_" + d];
    int q1 = c_controllability["CC0_" + clk] + c_controllability["CC1_" + clk] + c_controllability["CC1_" + d];
    if(q0>=MAXI) q0 = MAXI;
    if(q1>=MAXI) q1 = MAXI;
    return {q0,q1};
}

vector<int> ff_s_controllability(string clk, string d, string q) {
    int q0 = s_controllability["SC0_" + clk] + s_controllability["SC1_" + clk] + s_controllability["SC0_" + d] + 1;
    int q1 = s_controllability["SC0_" + clk] + s_controllability["SC1_" + clk] + s_controllability["SC1_" + d] + 1;    
    if(q0>=MAXI) q0 = MAXI;
    if(q1>=MAXI) q1 = MAXI;
    return {q0,q1};
}


void print_ctrl() {
    for(auto x:wire_list) {
        cout<<"CC0_"<<x<<": "<<c_controllability["CC0_"+x]<<" / ";
        cout<<"CC1_"<<x<<": "<<c_controllability["CC1_"+x]<<endl;
    }
    cout<<endl;
}

void print_s_ctrl() {
    for(auto x:wire_list) {
        cout<<"SC0_"<<x<<": "<<s_controllability["SC0_"+x]<<" / ";
        cout<<"SC1_"<<x<<": "<<s_controllability["SC1_"+x]<<endl;
    }
    cout<<endl;
}

void read_file() {
    string filename = "output_netlist.txt";
    ifstream file(filename);

    string line = "";
    while (getline(file, line)) {
        vector<string> tmp;
        for(int i=0;i<line.length();i++) {
            string s = "";
            while(i<line.length() && line[i]!= ' ') {
                s+=line[i];
                i++;
            }
            tmp.push_back(s);
        }
        netlist.push_back(tmp);        
    }

    file.close();

    set<string>wires;
    for(auto x:netlist) {
        if(x[0]=="FANOUT") {
            fanout.push_back(x);
            in_out_fanout.push_back(x);
        }
        else if(x[0]=="INPUT" || x[0]=="OUTPUT") {
            in_out_fanout.push_back(x);
            if(x[0]=="INPUT") {
                for(int i=1;i<x.size();i++) {
                    input_list.push_back(x[i]);
                }
            }
        } 
        else if(x[0]=="DFF") {
            ff_q.push_back(x[3]);
            ff_d.push_back(x[2]);
            // output_wires.push_back(x[3]);
            ff_list.push_back(x);
        }
        else {
            gate_list.push_back(x);
            output_wires.push_back(x[1]);
        }
        for(int i=1;i<x.size();i++) {
            wires.insert(x[i]);
        }
    }

    vector<string>tmp(wires.begin(),wires.end());
    wire_list = tmp;
    tmp.clear();
    // std::sort(output_wires.begin(),output_wires.end(),cmp);
    // std::sort(wire_list.begin(), wire_list.end(),cmp);


    for(auto x:wire_list) {
        string cc0 = "CC0_" + x;
        string cc1 = "CC1_" + x;
        string sc0 = "SC0_" + x;
        string sc1 = "SC1_" + x;
        c_controllability[cc0]=0;
        c_controllability[cc1]=0;
        s_controllability[sc0]=0;
        s_controllability[sc1]=0;
    }
    
    
    // for(auto x:netlist) {
    //     for(auto y:x) {
    //         cout<<y<<" ";
    //     }
    //     cout<<endl;
    // }

    cout<<"GATE_LIST::\n";
    for(auto x:gate_list) {
        for(auto y:x) {
            cout<<y<<" ";
        }
        cout<<endl;
    }
    cout<<endl;

    cout<<"FF_LIST::\n";
    for(auto x:ff_list) {
        for(auto y:x) {
            cout<<y<<" ";
        }
        cout<<endl;
    }
    cout<<endl;

    cout<<"\nFANOUT::\n";
    for(auto x:fanout) {
        for(auto y:x) {
            cout<<y<<" ";
        }
        cout<<endl;
    }
    cout<<endl;

    cout<<"\nIN_OUT_FANOUT::\n";
    for(auto x:in_out_fanout) {
        for(auto y:x) {
            cout<<y<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    
    cout<<"\nINPUT_LIST::\n";
    for(auto x:input_list) {
        cout<<x<<" ";
    }
    cout<<endl;
    
    cout<<"\nOUTPUT_WIRES::\n";
    for(auto x:output_wires) {
        cout<<x<<" ";
    }
    cout<<endl;
    
    cout<<"\nWIRE_LIST::\n";
    for(auto x:wire_list) {
        cout<<x<<" ";
    }
    cout<<endl;

    // for(auto x:c_controllability) {
    //     cout<<x.first<<" -> "<<x.second<<endl;
    // }
    // cout<<endl;
    
}


void ini() {
    for(auto x:input_list) {
        c_controllability["CC0_" + x] = 1 ;
        c_controllability["CC1_" + x] = 1 ;
        s_controllability["SC0_" + x] = 0 ;
        s_controllability["SC1_" + x] = 0 ;
    }
    for(auto x:ff_d) {
        c_controllability["CC0_" + x] = 1 ;
        c_controllability["CC1_" + x] = 1 ;
        s_controllability["SC0_" + x] = 0 ;
        s_controllability["SC1_" + x] = 0 ;
    }
    for(auto x:ff_q) {
        c_controllability["CC0_" + x] = MAXI;
        c_controllability["CC1_" + x] = MAXI;
        s_controllability["SC0_" + x] = MAXI;
        s_controllability["SC1_" + x] = MAXI;
    }

    cout<<"ITERATION -> 0"<<endl;
        print_ctrl();

    cout<<"ITERATION -> 0"<<endl;
        print_s_ctrl();
}

string find_gate(string gate_output_wire) {
    for(auto x:gate_list) {
        if(x[1]==gate_output_wire) {
            return x[0];
        }
    }
    return "";
}


vector<string> find_gate_list(string gate_output_wire) {
    for(auto x:gate_list) {
        if(x[1]==gate_output_wire) {
            return x;
        }
    }
    return {};
}

vector<string> find_gate_input(string gate_output_wire) {
    vector<string>g_list = find_gate_list(gate_output_wire);
    vector<string>i_list;
    for(int i=2;i<g_list.size();i++) {
        i_list.push_back(g_list[i]);
    }
    return i_list;
}


vector<int> c_controllability_output_of_gate_not_buf(string gate,string CC0_a,string CC1_a) {
    vector<int>z_out;
    int a0 = c_controllability[CC0_a];
    int a1 = c_controllability[CC1_a];
    int z0 = 0;
    int z1 = 0;
    if (gate == "NOT") {
        z0 = a1 + 1;
        z1 = a0 + 1;
    }
    else if (gate == "BUF") {
        z0 = a0 + 1;
        z1 = a1 + 1;
    }
    if(z0>=MAXI) z0 = MAXI;
    if(z1>=MAXI) z1 = MAXI;
    z_out.push_back(z0);
    z_out.push_back(z1);
    return z_out;
}

vector<int> c_controllability_output_of_gate(string gate,string CC0_a,string CC1_a,string CC0_b,string CC1_b ) {
    vector<int>z_out;
    int a0 = c_controllability[CC0_a];
    int a1 = c_controllability[CC1_a];
    int b0 = c_controllability[CC0_b];
    int b1 = c_controllability[CC1_b];
    int z0 = 0;
    int z1 = 0;
    if (gate == "AND") {
        z0 = min(a0, b0) + 1;
        z1 = a1 + b1 + 1;
    }
    else if (gate == "OR") {
        z0 = a0 + b0 + 1;
        z1 = min(a1, b1) + 1;
    }
    else if (gate == "NAND") {
        z0 = a1 + b1 + 1;
        z1 = min(a0, b0) + 1;
    }
    else if (gate == "NOR") {
        z0 = min(a1, b1) + 1;
        z1 = a0 + b0 + 1;
    }
    else if (gate == "XOR") {
        z0 = min(a0 + b0, a1 + b1) + 1;
        z1 = min(a1 + b0, a0 + b1) + 1;
    }
    else if (gate == "XNOR") {
        z0 = min(a1 + b0, a0 + b1) + 1;
        z1 = min(a0 + b0, a1 + b1) + 1;
    }
    if(z0>=MAXI) z0 = MAXI;
    if(z1>=MAXI) z1 = MAXI;
    z_out.push_back(z0);
    z_out.push_back(z1);
    return z_out;
}


vector<int> s_controllability_output_of_gate_not_buf(string gate,string SC0_a,string SC1_a) {
    vector<int>z_out;
    int a0 = s_controllability[SC0_a];
    int a1 = s_controllability[SC1_a];
    int z0 = 0;
    int z1 = 0;
    if (gate == "NOT") {
        z0 = a1 ;
        z1 = a0;
    }
    else if (gate == "BUF") {
        z0 = a0;
        z1 = a1;
    }
    if(z0>=MAXI) z0 = MAXI;
    if(z1>=MAXI) z1 = MAXI;
    z_out.push_back(z0);
    z_out.push_back(z1);
    return z_out;
}

vector<int> s_controllability_output_of_gate(string gate,string SC0_a,string SC1_a,string SC0_b,string SC1_b ) {
    vector<int>z_out;
    int a0 = s_controllability[SC0_a];
    int a1 = s_controllability[SC1_a];
    int b0 = s_controllability[SC0_b];
    int b1 = s_controllability[SC1_b];
    int z0 = 0;
    int z1 = 0;
    if (gate == "AND") {
        z0 = min(a0, b0);
        z1 = a1 + b1;
    }
    else if (gate == "OR") {
        z0 = a0 + b0 ;
        z1 = min(a1, b1) ;
    }
    else if (gate == "NAND") {
        z0 = a1 + b1 ;
        z1 = min(a0, b0) ;
    }
    else if (gate == "NOR") {
        z0 = min(a1, b1) ;
        z1 = a0 + b0 ;
    }
    else if (gate == "XOR") {
        z0 = min(a0 + b0, a1 + b1) ;
        z1 = min(a1 + b0, a0 + b1) ;
    }
    else if (gate == "XNOR") {
        z0 = min(a1 + b0, a0 + b1) ;
        z1 = min(a0 + b0, a1 + b1) ;
    }
    if(z0>=MAXI) z0 = MAXI;
    if(z1>=MAXI) z1 = MAXI;
    z_out.push_back(z0);
    z_out.push_back(z1);
    return z_out;
}

void set_fanouts() {
    for(auto x:fanout) {
        for(int i=2;i<x.size();i++) {
            c_controllability["CC0_" + x[i]] = c_controllability["CC0_" + x[1]];
            c_controllability["CC1_" + x[i]] = c_controllability["CC1_" + x[1]] ;
        }
    }
    // print_ctrl();
}


void calc1() {
    int iteration = 1;
    while(iteration>0) {
        for(int i=0;i<output_wires.size();i++) {
            string output_wire = output_wires[i];
            string gate = find_gate(output_wire);
            vector<string>input_list_of_gate = find_gate_input(output_wire);

            // for(auto y:input_list_of_gate) cout<<y<<" ";
            // cout<<endl;

            vector<int>z;
            if(gate=="NOT" || gate=="BUF") {
                string a0_str = "CC0_" + input_list_of_gate[0];
                string a1_str = "CC1_" + input_list_of_gate[0];
                z = c_controllability_output_of_gate_not_buf(gate,a0_str,a1_str);
            } else {
                string a0_str = "CC0_" + input_list_of_gate[0];
                string a1_str = "CC1_" + input_list_of_gate[0];
                string b0_str = "CC0_" + input_list_of_gate[1];
                string b1_str = "CC1_" + input_list_of_gate[1];
                z = c_controllability_output_of_gate(gate,a0_str,a1_str,b0_str,b1_str);
            }

            string z0_str = "CC0_" + output_wire;
            string z1_str = "CC1_" + output_wire;
            c_controllability[z0_str] = z[0];
            c_controllability[z1_str] = z[1];
            set_fanouts();
        }
        
        for(auto x:ff_list) {
            vector<int>z = ff_c_controllability(x[1],x[2],x[3]);
            c_controllability["CC0_" + x[3]] = z[0];
            c_controllability["CC1_" + x[3]] = z[1];
        }
        set_fanouts();
        
        cout<<"ITERATION -> "<<iteration<<endl;
        print_ctrl();
        
        bool f=0;
        for(auto x:c_controllability) {
            if( x.second==MAXI ) {
                f = 1;
                break;
            }
        }
        if(!f) iteration = 0;
        else iteration++;
    }
}


void calc2() {
    int iteration = 1;
    while(iteration>0) {
        for(int i=0;i<output_wires.size();i++) {
            string output_wire = output_wires[i];
            string gate = find_gate(output_wire);
            vector<string>input_list_of_gate = find_gate_input(output_wire);

            // for(auto y:input_list_of_gate) cout<<y<<" ";
            // cout<<endl;

            vector<int>z;
            if(gate=="NOT" || gate=="BUF") {
                string a0_str = "SC0_" + input_list_of_gate[0];
                string a1_str = "SC1_" + input_list_of_gate[0];
                z = s_controllability_output_of_gate_not_buf(gate,a0_str,a1_str);
            } else {
                string a0_str = "SC0_" + input_list_of_gate[0];
                string a1_str = "SC1_" + input_list_of_gate[0];
                string b0_str = "SC0_" + input_list_of_gate[1];
                string b1_str = "SC1_" + input_list_of_gate[1];
                z = s_controllability_output_of_gate(gate,a0_str,a1_str,b0_str,b1_str);
            }

            string z0_str = "SC0_" + output_wire;
            string z1_str = "SC1_" + output_wire;
            s_controllability[z0_str] = z[0];
            s_controllability[z1_str] = z[1];
            set_fanouts();
        }
        
        for(auto x:ff_list) {
            vector<int>z = ff_s_controllability(x[1],x[2],x[3]);
            s_controllability["SC0_" + x[3]] = z[0];
            s_controllability["SC1_" + x[3]] = z[1];
        }
        set_fanouts();
        
        cout<<"ITERATION -> "<<iteration<<endl;
        print_s_ctrl();
        
        bool f=0;
        for(auto x:s_controllability) {
            if( x.second==MAXI ) {
                f = 1;
                break;
            }
        }
        if(!f) iteration = 0;
        else iteration++;
    }
}


int main() {
    read_file();
    ini();
    calc1();
    calc2();

    return 0;
}