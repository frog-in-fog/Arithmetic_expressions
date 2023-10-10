#include <iostream>
#include<fstream>
#include <string>
#include <cctype>
#include <sstream>


template <typename T>
class Node {
public:

    Node* next;
    T value;

    explicit Node(T value = T(), Node* next = nullptr) {
        this->value = value;
        this->next = next;
    }
};


template <typename T>
class LinkedList {

public:
    LinkedList();
    ~LinkedList();

    virtual void push(T value);
    virtual T pop();

    int size() { return counter; }

    bool isEmpty();
    void clear();

    virtual T top();
    LinkedList(const LinkedList<T>& other);

    LinkedList<T>& operator=(const LinkedList<T>& other);
protected:
    int counter;
    Node<T>* head;
};


template <typename T>
LinkedList<T>::LinkedList(){
    counter = 0;
    head = nullptr;
}

template <typename T>
LinkedList<T>::~LinkedList(){
    if (isEmpty()) {
        delete head;
    }
    else {
        clear();
    }
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other){
    counter = other.counter;
    head = nullptr;

    Node<T>* otherNode = other.head;
    Node<T>* currentNode = nullptr;

    while (otherNode != nullptr) {
        if (head == nullptr) {
            head = new Node<T>(otherNode->value);
            currentNode = head;
        }
        else {
            currentNode->next = new Node<T>(otherNode->value);
            currentNode = currentNode->next;
        }

        otherNode = otherNode->next;
    }
}

template<typename T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T>& other) {
    {
        if (this == &other) {
            return *this;
        }

        clear();

        counter = other.counter;
        head = nullptr;

        Node<T>* otherNode = other.head;
        Node<T>* currentNode = nullptr;

        while (otherNode != nullptr) {
            if (head == nullptr) {
                head = new Node<T>(otherNode->value);
                currentNode = head;
            }
            else {
                currentNode->next = new Node<T>(otherNode->value);
                currentNode = currentNode->next;
            }

            otherNode = otherNode->next;
        }

        return *this;
    }
};

template <typename T>
void LinkedList<T>::push(T value)
{
    if (head == nullptr) {
        head = new Node<T>(value);
    }
    else {
        Node<T>* temp = head;
        head = new Node<T>(value, temp);
    }
    counter++;
}

template <typename T>
bool LinkedList<T>::isEmpty() {
    return head == nullptr;
}


template <typename T>
T LinkedList<T>::pop() {
    if (this->isEmpty()) {
        std::cout << "Stack is empty" << std::endl;
        return 0;
    }
    else {
        Node<T>* temp = this->head;
        T value = head->value;
        head = head->next;
        delete temp;
        counter--;
        return value;
    }
}

template <typename T>
void LinkedList<T>::clear() {
    if (head != nullptr) {
        int Local_List_Size = counter;
        for (size_t i = 0; i < Local_List_Size; i++){
            pop();
        }
    }

}

template <typename T>
T LinkedList<T>::top() {
    if (this->isEmpty()) {
        return 0;
    }
    else {
        return head->value;
    }
}

template<typename T>
class Stack : public LinkedList<T> {
public:
    Stack(): LinkedList<T>(){}
    ~Stack(){
        LinkedList<T>::clear();
    }

    Stack(const Stack<T>& other): LinkedList<T>(other){}

    Stack& operator = (const Stack& other){
        LinkedList<T>::operator=(other);
    }

    T top() override{
        if (this->isEmpty()){
            return 0;
        } else {
            return this->head->value;
        }
    }
};

int getPriority(char o) {
    switch (o) {
        case '(': return 1;
        case '+': case '-': return 2;
        case '*': case '/': case '%': return 3;
        case '^': return 4;
        default: return 0;
    }
}


int getRes(std::string& str_out) {
    Stack<int> result;
    std::string tempstr{};
    int value = 0;
    int num1, num2;
    int res = 0;

    for (size_t i = 0; i < str_out.size(); i++)
    {
        while ((str_out[i] != ' ') && (isdigit(str_out[i]))) {
            tempstr += str_out[i];
            i++;
        }
        if (!tempstr.empty()) {
            value = stoi(tempstr);
            result.push(value);
            tempstr.clear();
        }
        if (str_out[i] != ' ') {
            num1 = result.pop();
            num2 = result.pop();
            switch (str_out[i]) {
                case '+': res = num1 + num2; break;
                case '-': res = num2 - num1; break;
                case '*': res = num1 * num2; break;
                case '/': res = num2 / num1; break;
                case '%': res = num2 % num1; break;
                case '^': res = num2 ^ num1; break;
                default: throw (std::string)"Error";
            }
            result.push(res);
        }

    }
    return result.pop();

}
std::string reversePolish(const std::string& str) {
    int isOp = 0, np = 0;
    bool isDigit = false;
    Stack<char> operationStack;
    std::string str_in = str;
    std::string str_out{};

    if ((!isdigit(str_in[0])) && str_in[0] != '(')
        throw (std::string)str + " " + "Expression should start from num or opening bracket";

    for (char ch : str_in) {
        if (isdigit(ch)) {
            str_out += ch;
            isOp = 0;
            isDigit = true;
        }
        else str_out += ' ';

        if (!isDigit) {
            switch (ch) {
                case '(':
                    operationStack.push(ch);
                    ++np;
                    isOp = 0;
                    break;
                case '*': case '/': case '+': case '-': case '^': case '%':
                    if (!isOp) {
                        isOp = 1;
                        while (getPriority(ch) <= getPriority(operationStack.top())) {
                            str_out += operationStack.pop();
                            str_out += ' ';
                        }
                        if (getPriority(ch) > getPriority(operationStack.top())) {
                            operationStack.push(ch);
                        }
                        break;
                    }
                    else throw (std::string)str + " " + "Several operations in a row";

                case ')':
                    if (isOp)
                        throw (std::string)str + " " + "Opening bracket not found";
                    else
                        while ((ch = operationStack.pop()) != '(' && np > 0) {

                            str_out += ch;
                            str_out += ' ';
                        }
                    --np;
                    break;

                case ' ':
                    //do nothing
                    break;

                default: {
                    throw (std::string)str + " " + "Incorrect symbol in string";
                }
            }
        }
        else {
            isDigit = false;
        }
    }
    while (operationStack.size() != 0) {
        str_out += ' ';
        str_out += operationStack.pop();
    }
    if (np)
        throw (std::string)str + " " + "Incorrect count of brackets";
    return str_out;
}

int errorFileIndex = 0;
std::string generateErrorFileName(std::string& inputfile) {
    std::stringstream fileNameStream;
    fileNameStream << "" << inputfile << ' ' << errorFileIndex++ << ".txt";
    std::string fileName = fileNameStream.str();

    return fileName;
}
void writeToErrorFile(std::string& exception, int line_counter, std::string& input, std::string& out) {


    std::ofstream targetFile(out, std::ios::app);
    if (!targetFile.is_open()) {
        std::cerr << "Error creating file " << out << "." << std::endl;

    }
    else {
        targetFile << line_counter << std::endl;
        targetFile << exception << std::endl;
    }
    targetFile.close();

}

void handleFiles(std::string& input)
{
    int line_counter = 0;
    std::ifstream input_file(input);
    bool Exception = false;
    std::string str_out{};
    std::string Error_File_Name = generateErrorFileName(input);
    while (!Exception) {
        if (!input_file.is_open())
        {
            Exception = true;
            std::cerr << "Error with opening file\n";
        }
        try
        {
            std::string line;
            while (std::getline(input_file, line))
            {
                try {
                    str_out = reversePolish(line);

                    std::cout << "Given string: " << line << std::endl;
                    std::cout << "Postfix form: " << str_out << std::endl;
                    std::cout << "Result: " << getRes(str_out) << std::endl;
                }
                catch (std::string exception) {
                    writeToErrorFile(exception, line_counter, input, Error_File_Name);
                }
                line_counter++;
            }
            Exception = true;
        }
        catch (std::string e)
        {
            std::cerr << "Exception: " << e << '\n';
            Exception = true;
        }
    }
    input_file.close();

}


int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Enter the name of file...";
    } else {
        for (size_t i = 1; i < argc; i++){
            std::string filename = argv[i];
            handleFiles(filename);
        }

    }
}