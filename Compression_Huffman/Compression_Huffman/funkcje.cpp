#include <fstream>
#include <iostream>
#include <queue>
#include <bitset>

#include "funkcje.h"


void usage()
{
    std::cout << "Podano nieprawidlowe parametry!" << std::endl;
    std::cout << "-t tryb(k - kompresja, d - dekompresja) -i sciezka_pliku_wejsciowego -s sciezka_slownika -o sciezka_pliku_wyjsciowego" << std::endl;
}

void getParams(int argc, char* argv[], options& data)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i] == std::string("-i"))
        {
            if ((i + 1) < argc)
                data.input = argv[++i];
        }
        else if (argv[i] == std::string("-o"))
        {
            if ((i + 1) < argc)
                data.output = argv[++i];
        }
        else if (argv[i] == std::string("-t"))
        {
            if ((i + 1) < argc)
                data.tryb = argv[++i];
        }
        else if (argv[i] == std::string("-s"))
        {
            if ((i + 1) < argc)
                data.dictionary = argv[++i];
        }
    }
}

Node* createNode(char character, int value, Node* left, Node* right)
{
    Node* node = new Node(character, value, left, right);
    return node;
}

bool isOnly(Node* root)
{
    return root->left == nullptr && root->right == nullptr;
}

void searchTree(Node* root, std::string str, std::unordered_map<char, std::string>& huffmanCode)
{
    if (root == nullptr)
    {
        return;
    }

    if (isOnly(root))
    {
        huffmanCode[root->character] = str;
    }

    searchTree(root->left, str + "0", huffmanCode);
    searchTree(root->right, str + "1", huffmanCode);
}

void compress(options opt)
{
    const std::string empty = "";

    std::ifstream file(opt.input);
    std::string text;

    if (file.good() == false)
    {
        std::cout << "Plik nie zostal znaleziony!\n";
        exit(0);
    }

    while (getline(file, text))
        file.close();

    std::unordered_map<unsigned char, int> frequency;

    for (auto i : text)
    {
        frequency[i]++;
    }
    
    std::priority_queue<Node*, std::vector<Node*>, compare> prior_q ; // compare to funktor

    for (auto pair : frequency)
    {
        prior_q.push(createNode(pair.first, pair.second, nullptr, nullptr));
    }

    while (prior_q.size() != 1)
    {
        Node* left = prior_q.top();
        prior_q.pop();
        Node* right = prior_q.top();
        prior_q.pop();

        int sum = left->value + right->value;

        prior_q.push(createNode('\0', sum, left, right));
    }

    Node* root = prior_q.top();

    std::unordered_map<char, std::string> huffmanCode;

    searchTree(root, empty, huffmanCode);

    std::fstream dictionary(opt.dictionary, std::ios::out);

    for (auto pair : huffmanCode)
    {
        dictionary.put(pair.first);
        dictionary.put('\n');
        dictionary.write(pair.second.c_str(), pair.second.size());
        dictionary.put('\n');
       
    }

    std::string encoded;

    for (char ch : text)
    {
        encoded += huffmanCode[ch];
    }

    std::string decoded_c;
    std::vector<char> buffer;
    int displace;

    convert(encoded, buffer, displace);

    std::cout << std::endl;

    displace += 48;

    dictionary.close();

    std::fstream encoded_file(opt.output, std::ios::out | std::ios::binary);
    encoded_file.put(displace);
    copy(buffer.begin(), buffer.end(), std::ostreambuf_iterator<char>(encoded_file));
    encoded_file.close();

    delete root;

}

void decompress(options opt)
{
    std::vector<std::pair<char, std::string>>huffmanCode_D;
    std::string str_dict;
    char ch_dict;
    std::string str2_dict;

    std::fstream dictionary_D(opt.dictionary, std::ios::in);
    
    if (dictionary_D.good() == false)
    {
        std::cout << "Slownik nie zostal znaleziony!" << std::endl;
        exit(0);
    }

    int x = 1;

    while (getline(dictionary_D, str_dict))
    {
        if (x % 2)
        {
            ch_dict = str_dict[0];
        }
        else
        {
            str2_dict = str_dict;
            huffmanCode_D.push_back(make_pair(ch_dict, str2_dict));
        }
        x++;
    }
    dictionary_D.close();

    std::string to_encode;
    
    std::fstream decode_file(opt.input, std::ios::in);

    if (decode_file.good() == false)
    {
        std::cout << "Plik nie zostal znaleziony!" << std::endl;
        exit(0);
    }

    while (getline(decode_file, to_encode))
        decode_file.close();

    std::string displace_str = to_encode.substr(0, 1);

    int displace_int = 0;

    displace_int = atoi(displace_str.c_str());

    to_encode.erase(to_encode.begin(), to_encode.begin() + 1);

    convertString(to_encode);

    to_encode.erase(to_encode.end() - (8 - displace_int), to_encode.end());
    
    std::string huff_str;
    std::string decoded;

    for (int i = 0; i < to_encode.size(); i++)
    {
        huff_str += to_encode[i];
        for (int j = 0; j < huffmanCode_D.size(); j++)
        {
            if (huff_str == huffmanCode_D.at(j).second)
            {
                decoded += huffmanCode_D.at(j).first;
                huff_str.clear();
                break;
            }
        }
    }

    std::fstream saved_file(opt.output, std::ios::out | std::ios::binary);
    saved_file << decoded;
    saved_file.close();

}

void convert(std::string encoded, std::vector<char>& buffer, int& displace)
{
    int counter = 0;

    static unsigned char bit_c = 0;

    for (unsigned char bit : encoded)
    {
        if (counter > 7)
        {
            buffer.push_back(bit_c);
            counter = 0;
        }
        if (bit == '0')
        {
            bit_c = (bit_c << 1) & 0xFE;
        }
        else
        {
            bit_c= (bit_c << 1) | 0x01;
        }
        counter++;
    }

    displace = counter;

    if (counter > 0)
    {
        bit_c = (bit_c << (8 - displace));
        buffer.push_back(bit_c);
    }
}

void convertString(std::string& to_encode)
{
    std::string decoded_str;

    for (auto ch : to_encode)
    {
        decoded_str += std::bitset<8>(ch).to_string();
    }
    
    to_encode = decoded_str;
}