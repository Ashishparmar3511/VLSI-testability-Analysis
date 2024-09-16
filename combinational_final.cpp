#include<bits/stdc++.h>
using namespace std;


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
vector<vector<string>> gate_list;
vector<vector<string>> fanout;
vector<string>input_list;
vector<string>output_wires;
vector<string>wire_list;
vector<string>gate_input_list;
map<string,int>controllability;
map<string,int>observability;



void print_ctrl() {
    for(auto x:wire_list) {
        cout<<"CC0_"<<x<<": "<<controllability["CC0_"+x]<<" / ";
        cout<<"CC1_"<<x<<": "<<controllability["CC1_"+x]<<endl;
    }
    cout<<endl;
}

void read_file() {
    string filename;
    // cin>>filename;
    filename = "output_netlist.txt";
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
    std::sort(output_wires.begin(),output_wires.end(),cmp);
    std::sort(wire_list.begin(), wire_list.end(),cmp);


    for(auto x:wire_list) {
        string cc0 = "CC0_" + x;
        string cc1 = "CC1_" + x;
        controllability[cc0]=0;
        controllability[cc1]=0;
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

    // for(auto x:controllability) {
    //     cout<<x.first<<" -> "<<x.second<<endl;
    // }
    // cout<<endl;
    
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


vector<int> controllability_output_of_gate_not_buf(string gate,string CC0_a,string CC1_a) {
    vector<int>z_out;
    int a0 = controllability[CC0_a];
    int a1 = controllability[CC1_a];
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
    z_out.push_back(z0);
    z_out.push_back(z1);
    return z_out;
}



vector<int> controllability_output_of_gate(string gate,string CC0_a,string CC1_a,string CC0_b,string CC1_b ) {
    vector<int>z_out;
    int a0 = controllability[CC0_a];
    int a1 = controllability[CC1_a];
    int b0 = controllability[CC0_b];
    int b1 = controllability[CC1_b];
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
    z_out.push_back(z0);
    z_out.push_back(z1);
    return z_out;
}

void set_fanouts() {
    for(auto x:fanout) {
        for(int i=2;i<x.size();i++) {
            controllability["CC0_" + x[i]] = controllability["CC0_" + x[1]];
            controllability["CC1_" + x[i]] = controllability["CC1_" + x[1]] ;
        }
    }
    // print_ctrl();
}


void calc0() {
    for(auto x:input_list) {
        controllability["CC0_" + x] = 1 ;
        controllability["CC1_" + x] = 1 ;
    }
    set_fanouts();
    // print_ctrl();
}




void calc1() {
    for(int i=0;i<output_wires.size();i++) {
        string output_wire = output_wires[i];
        string gate = find_gate(output_wire);
        vector<string>input_list_of_gate = find_gate_input(output_wire);

        for(auto y:input_list_of_gate) cout<<y<<" ";
        cout<<endl;

        vector<int>z;
        if(gate=="NOT" || gate=="BUF") {
            string a0_str = "CC0_" + input_list_of_gate[0];
            string a1_str = "CC1_" + input_list_of_gate[0];
            z = controllability_output_of_gate_not_buf(gate,a0_str,a1_str);
        } else {
            string a0_str = "CC0_" + input_list_of_gate[0];
            string a1_str = "CC1_" + input_list_of_gate[0];
            string b0_str = "CC0_" + input_list_of_gate[1];
            string b1_str = "CC1_" + input_list_of_gate[1];
            z = controllability_output_of_gate(gate,a0_str,a1_str,b0_str,b1_str);
        }

        string z0_str = "CC0_" + output_wire;
        string z1_str = "CC1_" + output_wire;
        controllability[z0_str] = z[0];
        controllability[z1_str] = z[1];
        set_fanouts();
    }
}

// ===============================================================================


void gate_in_list() {
    for(auto x:gate_list) {
        for(int i=2;i<x.size();i++) {
            gate_input_list.push_back(x[i]);
        }
    }
    sort(gate_input_list.begin(),gate_input_list.end(),cmp2);

    cout<<"GATE_INPUT_LIST::  ";
    for(auto x:gate_input_list) {
        cout<<x<<" ";
    }
    cout<<endl;
}


string return_out_of_in(string input) {
    for(auto x:gate_list) {
        for(int i=2;i<x.size();i++) {
            if(x[i]==input) {
                return x[1];
            }
        }
    }
    return "";
}


vector<string> return_other_in(string output) {
    vector<string> i_list;
    for(auto x:gate_list) {
        if(x[1]==output) {
            for(int i=2;i<x.size();i++) {
                i_list.push_back(x[i]);
            }
            return i_list;
        }
    }
    return {};
}


void set_fanouts_observability() {
    for(auto x:fanout) {
        int z_mini = INT_MAX;
        for(int i=2;i<x.size();i++) {
            z_mini = min(z_mini,observability["CO_" + x[i]]);
        }
        observability["CO_" + x[1]] = z_mini;
    }
}


int observability_output_of_gate_not_buf(string gate,string CO_a) {
    int z_out;
    int a = observability[CO_a];
    // if (gate == "NOT") {
    //     z = a + 1;
    // }
    // else if (gate == "BUF") {
    //     z = a + 1;
    // }
    return z_out = a + 1;
}


int observability_output_of_gate(string gate,int CC0_b,int CC1_b, int z) {
    int za = 0;
    if (gate == "AND") {
        za = z + CC1_b + 1;
    } else if (gate == "OR") {
        za = z + CC0_b + 1;
    } else if (gate == "NAND") {
        za = z + CC1_b + 1;
    } else if (gate == "NOR") {
        za = z + CC0_b + 1;
    } else if (gate == "XOR" || gate == "XNOR") {
        za = z + min(CC0_b, CC1_b) + 1;
    }
    return za;
}


void calc2() {
    for(auto x:wire_list) {
        observability["CO_" + x] = 0 ;
    }
    // set_fanouts();
    // print_ctrl();
}



void calc3() {
    for(auto x:gate_input_list) {
        string ip1 = x;
        string output_wire = return_out_of_in(ip1);
        vector<string> input_list_of_gate = return_other_in(output_wire);
        string gate = find_gate(output_wire);

        int z_out = observability["CO_" + output_wire];
        if(gate=="NOT" || gate=="BUF") {
            string a_str = "CO_" + input_list_of_gate[0];
            observability[a_str] = z_out + 1;
        } else {
            int CC0_a = controllability["CC0_"+input_list_of_gate[0]];
            int CC1_a = controllability["CC1_"+input_list_of_gate[0]];
            int CC0_b = controllability["CC0_"+input_list_of_gate[1]];
            int CC1_b = controllability["CC1_"+input_list_of_gate[1]];
            string a_str = "CO_" + input_list_of_gate[0];
            string b_str = "CO_" + input_list_of_gate[1];
            int za = observability_output_of_gate(gate,CC0_b,CC1_b,z_out);
            int zb = observability_output_of_gate(gate,CC0_a,CC1_a,z_out);
            observability[a_str] = za;
            observability[b_str] = zb;
        }
        set_fanouts_observability();
    }
}


void print_obs() {
    for(auto x:wire_list) {
        cout<<"CO_"<<x<<": "<<observability["CO_"+x]<<endl;
    }
    cout<<endl;
}


int main() {
    read_file();
    calc0();
    calc1();
    print_ctrl();

    gate_in_list();
    calc2();
    calc3();
    print_obs();
    return 0;
}