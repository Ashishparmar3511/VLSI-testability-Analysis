#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <cstring>
#include <string>
#include <unordered_map>
using namespace std;


struct Gate // the structure to store the data of gate
{
    string type;
    string g_name;
    int level = 0;
    vector<string> inputs;
    vector<string> outputs;
};


struct Module // to store the whole module's basic information
{
    vector<string> inputs;
    vector<string> outputs;
    vector<Gate> gates;
};

map<string, vector<string>> fanout;

Module parseVerilog(const string &filename) // function to fetch Verilog file and return the Module
{
    ifstream file(filename);
    string line;
    regex inputRegex(R"(input\s+([a-zA-Z0-9_,\s]+);)");
    regex wireRegex(R"(wire\s+([a-zA-Z0-9_,\s]+);)");
    regex outputRegex(R"(output\s+([a-zA-Z0-9_,\s]+);)");
    regex nandRegex(R"(nand\s+([()a-zA-Z0-9_,\s]+);)");
    regex andRegex(R"(and\s+([()a-zA-Z0-9_,\s]+);)");
    regex orRegex(R"(or\s+([()a-zA-Z0-9_,\s]+);)");
    regex xorRegex(R"(xor\s+([()a-zA-Z0-9_,\s]+);)");
    regex xnorRegex(R"(xnor\s+([()a-zA-Z0-9_,\s]+);)");
    regex norRegex(R"(nor\s+([()a-zA-Z0-9_,\s]+);)");
    regex notRegex(R"(not\s+([()a-zA-Z0-9_,\s]+);)");
    regex bufRegex(R"(buf\s+([()a-zA-Z0-9_,\s]+);)");
    regex dffRegex(R"(dff\s+([()a-zA-Z0-9_,\s]+);)");
    

    Module module;

    while (getline(file, line))
    {
        smatch match;

        if (regex_search(line, match, inputRegex))
        {
            string inputsStr = match[1].str();

            stringstream inputsStream(inputsStr);
            string input;
            while (getline(inputsStream, input, ','))
            {
                module.inputs.push_back(input);
            }
        }
        else if (regex_search(line, match, wireRegex))
        {
            string inputsStr = match[1].str();
            stringstream inputsStream(inputsStr);
            string input;
            while (getline(inputsStream, input, ','))
            {
                module.outputs.push_back(input);
            }
        }
        else if (regex_search(line, match, outputRegex))
        {
            string outputsStr = match[1].str();
            stringstream outputsStream(outputsStr);
            string output;
            while (getline(outputsStream, output, ','))
            {
                module.outputs.push_back(output);
            }
        }
        else if (regex_search(line, match, nandRegex))
        {
            Gate gate;
            gate.type = "nand";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
            // mp[gate.outputs[0]]
        }
        else if (regex_search(line, match, andRegex))
        {
            Gate gate;
            gate.type = "and";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
        }
        else if (regex_search(line, match, xnorRegex))
        {
            Gate gate;
            gate.type = "xnor";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
        }
        else if (regex_search(line, match, xorRegex))
        {
            Gate gate;
            gate.type = "xor";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
        }
        else if (regex_search(line, match, norRegex))
        {
            Gate gate;
            gate.type = "nor";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
        }
        else if (regex_search(line, match, orRegex))
        {
            Gate gate;
            gate.type = "or";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
        }
        else if (regex_search(line, match, notRegex))
        {
            Gate gate;
            gate.type = "not";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            module.gates.push_back(gate);
        }
        else if (regex_search(line, match, bufRegex))
        {
            Gate gate;
            gate.type = "buf";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
           
            module.gates.push_back(gate);
        }
        else if(regex_search(line, match, dffRegex)){
            cout << "HI"<< endl;
            Gate gate;
            gate.type = "dff";
            string inputsStr = match[1].str();
            inputsStr.erase(remove(inputsStr.begin(), inputsStr.end(), ' '), inputsStr.end());
            stringstream inputsStream(inputsStr);
            string input;
            string ss;
            getline(inputsStream, ss, '(');
            gate.g_name = ss;
            while (getline(inputsStream, input, ','))
                gate.inputs.push_back(input);
            string sss = gate.inputs.back();
            sss = sss.substr(0, sss.size() - 1);
            gate.inputs[gate.inputs.size() - 1] = sss;
            if (gate.inputs.size() != 0)
            {
                gate.outputs.push_back(gate.inputs[0]);
                gate.inputs.erase(gate.inputs.begin());
            }
            // vector<string> temp = gate.inputs;
            // gate.inputs = gate.outputs;
            // gate.outputs =temp;
            module.gates.push_back(gate);
        }
    }
    
    // cout << "BYE" << endl;

    
    return module;
}

void generateNetlist( Module &module,  const string &filename)
{
    //   for ( auto& gate : module.gates) {
    //     for ( auto& i : gate.inputs) {
    //         // If we find the same input in other gates, then we have a fanout
    //         // We rename the fanout branches as originalName_counter
    //         // Then, we create a new gate called a fanout gate
    //         int counter = 0;
    //         string originalLineName = i;
    //         bool foundFanout = false;

    //         for ( auto& otherGate : module.gates) {
    //             if (find(otherGate.inputs.begin(), otherGate.inputs.end(), i) != otherGate.inputs.end()) {
    //                 if (otherGate.g_name != gate.g_name) {
    //                     foundFanout = true;
    //                     break;
    //                 } else {
    //                     // The same gate could for whatever reason have a fanout of the same line, we try to detect that
    //                     if (count(otherGate.inputs.begin(), otherGate.inputs.end(), i) > 1) {
    //                         foundFanout = true;
    //                         break;
    //                     }
    //                 }
    //             }
    //         }

    //         if (foundFanout) {
    //             // Find and replace everywhere
    //             // And create a fanout gate
    //             Gate fanoutGate;
    //             fanoutGate.type = "fanout";
    //             fanoutGate.inputs.push_back(originalLineName);
    //             for (auto& g : module.gates) {
    //                 if (find(g.inputs.begin(), g.inputs.end(), originalLineName) != g.inputs.end() && g.type != "fanout") {
    //                     for (size_t index = 0; index < g.inputs.size(); ++index) {
    //                         if (g.inputs[index] == originalLineName) {
    //                             g.inputs[index] = originalLineName + "_" + to_string(counter);
    //                             ++counter;
    //                             fanoutGate.outputs.push_back(g.inputs[index]);
    //                         }
    //                     }
    //                 }
    //             }
    //             module.gates.push_back(fanoutGate);
    //         }
    //     }
    // }

    ofstream outfile(filename);

    // Write inputs
    outfile << "INPUT ";
    for (const auto &input : module.inputs)
    {
        outfile << input << " ";
    }
    outfile << "\n";

    // Write outputs
    outfile << "OUTPUT ";
    for (const auto &output : module.outputs)
    {
        outfile << output << " ";
    }
    outfile << "\n";

    // Logic Levelizing
    // unordered_map<string, int> gateLevels;
    // for (const auto &gate : module.gates)
    // {
    //     int maxLevel = 0;
    //     for (const auto &input : gate.inputs)
    //     {
    //         if (gateLevels.find(input) != gateLevels.end())
    //         {
    //             maxLevel = max(maxLevel, gateLevels[input]);
    //         }
    //     }
    //     gateLevels[gate.g_name] = maxLevel + 1;
    // }

    // // Update gate levels
    // for (auto gate : module.gates)
    // {
    //     gate.level = gateLevels[gate.g_name];
    // }

    // Sort gates based on level
    vector<Gate> sortedGates = module.gates;
    // sort(sortedGates.begin(), sortedGates.end(), [&](const Gate &a, const Gate &b)
    //      { return a.level < b.level; });

    // Write sorted gates
    for (const auto &gate : sortedGates)
    {
        string str = gate.type;
        transform(str.begin(), str.end(), str.begin(), ::toupper);
        outfile << str << " ";
        outfile << gate.outputs[0] << " ";
        for (const auto &input : gate.inputs)
        {
            outfile << input << " ";
        }
        outfile << "\n";
    }

    outfile.close();
}

void displayGates(const Module &module)
{
    cout << "Gate Details:\n";
    for (const auto &gate : module.gates)
    {
        cout << "Type: " << gate.type << endl;
        cout << "Inputs:";
        for (const auto &input : gate.inputs)
        {
            cout << " " << input;
        }
        cout << endl;
        cout << "Outputs:";
        for (const auto &output : gate.outputs)
        {
            cout << " " << output;
        }
        cout << endl;
        cout << "Level: " << gate.level << endl;
        cout << endl;
    }
}




void levelize(Module &module)
{
    map<string, int> gatelevels;
    map<string, int> netLevels;
    set<string> readyGates;
    set<string> readyNets;

    // Set level of primary inputs to 0
    for (const auto &input : module.inputs)
    {
        netLevels[input] = 0;
        readyNets.insert(input);
    }

    // Set level of constant signals to 0
   
    while (!readyGates.empty() || !readyNets.empty())
    {
        if (!readyGates.empty())
        {

            string gname = *readyGates.begin();
            Gate *gt;
            readyGates.erase(gname);
            for (auto &it : module.gates)
            {
                if (it.g_name == gname)
                {

                    int maxLevel = 0;
                    for (auto &input : it.inputs)
                    {
                        maxLevel = max(maxLevel, netLevels[input]);
                    }

                    it.level = maxLevel + 1;
                    gatelevels[it.g_name] = maxLevel + 1;

                    for (auto &output : it.outputs)
                    {
                        netLevels[output] = it.level;
                        readyNets.insert(output);
                    }
                }
            }
        }

        if (!readyNets.empty())
        {
            string netName = *readyNets.begin();
            readyNets.erase(netName);

            int maxLevel = 0;
            // for ( auto &gate : module.gates)
            // {
            //     if (find(gate.inputs.begin(), gate.inputs.end(), netName) != gate.inputs.end())
            //     {
            //         maxLevel = max(maxLevel, gate.level);
            //     }


            // }

            // netLevels[netName] = maxLevel;

            for (auto &gate : module.gates)
            {
                if (find(gate.inputs.begin(), gate.inputs.end(), netName) != gate.inputs.end())
                {
                    bool allInputsHaveLevels = true;
                    // int maxlevel = -1;
                    for (const auto &input : gate.inputs)
                    {
                        if (netLevels.find(input) == netLevels.end())
                        {
                            allInputsHaveLevels = false;
                            break;
                        }

                        else
                        {
                            maxLevel = max(maxLevel, netLevels[input]);
                        }
                    }
                    if (allInputsHaveLevels)
                    {
                        gate.level = maxLevel+1;
                        gatelevels[gate.g_name] = maxLevel+1;
                        readyGates.insert(gate.g_name);
                    }
                }
            }
        }
    }

    for(auto it: gatelevels){
        cout << it.first << " " << it.second << endl;
    }

}




int main()
{
    string file_name;
    cout << "Enter the file name you want to make module: ";
    cin >> file_name;

    Module module = parseVerilog(file_name);       // fetching the verilog file
    // levelize(module);
    generateNetlist(module,"output_netlist.txt"); // Generate netlist file

    cout << "Netlist file generated successfully!\n";
    displayGates(module);

    return 0;
}
