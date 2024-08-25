  #include "Controller.h"

string Control::toStr()
{
    string str;
    switch (type)
    {
        case DELTA:
            str = "<D" + std::to_string(index) + ">"; // Delta node
            return str;
        case NAME:
            return variables.at(0); // Variable name
        case LAMBDA:
            str = "[lambda closure: ";
            for (int i = 0; i < variables.size(); i++)
                str += variables.at(i) + ": "; // Lambda closure with variables
            str += to_string(index) + "]";
            return str;
        case GAMMA:
            return "Gamma"; // Gamma node
        case AUG:
            return "AUG"; // AUG node
        case BETA:
            return "BETA"; // BETA node
        case OR:
            return "OR"; // Logical OR
        case AND_LOGICAL:
            return "AND"; // Logical AND
        case NOT:
            return "NOT"; // Logical NOT
        case GR:
            return ">"; // Greater than
        case GE:
            return ">="; // Greater than or equal to
        case LS:
            return "<"; // Less than
        case LE:
            return "<="; // Less than or equal to
        case EQ:
            return "="; // Equal
        case NE:
            return "!="; // Not equal
        case ADD:
            return "+"; // Addition
        case SUBTRACT:
            return "-"; // Subtraction
        case NEG:
            return "NEG"; // Negation
        case MULTIPLY:
            return "*"; // Multiplication
        case DIVIDE:
            return "/"; // Division
        case EXP:
            return "**"; // Exponentiation
        case AT:
            return "@"; // At symbol
        case TRUE:
            return "true"; // Boolean true
        case FALSE:
            return "false"; // Boolean false
        case NIL:
            return "nil"; // Nil value
        case DUMMY:
            return "dummy"; // Dummy value
        case YSTAR:
            return "Y"; // Y-star node
        case TAU:
            str = "<T" + to_string(index) + ">"; // Tau node
            return str;
        case STRING:
            return ctrlVal + ""; // String value
        case INTEGER:
            str = ctrlVal + ""; // Integer value
            return str;
        case ENV:
            str = "e" + to_string(index); // Environment variable
            return str;
        case ETA:
            str = "<ETA," + to_string(index);
            for (int i = 0; i < variables.size(); i++)
                str += "," + variables.at(i); // Eta node with variables
            str += ">";
            return str;
        case TUPLE:
            str = "(";
            for (int i = 0; i < ctrlTuples.size(); i++)
            {
                str += ctrlTuples.at(i)->toStr(); // Tuple elements
                if (i != ctrlTuples.size() - 1)
                    str += ", ";
            }
            str += ")";
            return str;
        default:
            return "unknown"; // Unknown node type
    }
}

// Constructor for copying a Control object
Control::Control(Control *cntrl)
{
    index = cntrl->index;
    ctrlVal = cntrl->ctrlVal;
    type = cntrl->type;
    associatedENV = cntrl->associatedENV;
    if (!cntrl->variables.empty())
    {
        for (int i = 0; i < cntrl->variables.size(); i++)
            variables.push_back(cntrl->variables.at(i));
    }
    if (!cntrl->ctrlTuples.empty())
    {
        for (int i = 0; i < cntrl->ctrlTuples.size(); i++)
            ctrlTuples.push_back(cntrl->ctrlTuples.at(i));
    }
}

// Constructor for DELTA type Control
Control::Control(Control::Type type, int index)
{
    if (Control::DELTA != type)
    {
        printf("Control::DELTA's constructor called for: %d", type);
    }
    this->type = type;
    this->index = index;
    ctrlStruct = new vector<Control *>();
}

// Constructor for other Control types with variables
Control::Control(Control::Type type, vector<string> *vars, Control *del_ptr, int deltaindex)
{
    this->type = type;
    index = deltaindex;
    if (vars != NULL)
    {
        for (int i = 0; i < vars->size(); i++)
        {
            variables.push_back(vars->at(i));
        }
    }
}

// Constructor for TAU and ENV types
Control::Control(Control::Type type, int index, bool watever)
{
    if (type != Control::TAU && type != Control::ENV)
    {
        printf("Control::TAU's constructor called for: %d", type);
    }
    this->type = type;
    this->index = index;
}

// Constructor for single variable value
Control::Control(string var_value, Control::Type type)
{
    this->type = type;
    variables.push_back(var_value);
}

// Constructor for string value
Control::Control(Control::Type type, string value)
{
    this->type = type;
    ctrlVal = value;
}

// Default constructor
Control::Control()
{
}

// Constructor for Control type
Control::Control(Control::Type type)
{
    this->type = type;
}

// Print function for DELTA type Control
void Control::std_print()
{
    if (type != Control::DELTA)
    {
        printf("Error");
    }
    cout << toStr() << " ";
    for (int i = 0; i < ctrlStruct->size(); i++)
    {
        cout << ctrlStruct->at(i)->toStr() << " ";
    }
    cout << endl;
}

void Control::addCtrl(treeNode* node, int type, string value, vector<string>* variables, Control* del_ptr, int deltas_size)
{
    int tau_count;
    Control* temp = NULL;
    switch (node->type)
    {
        case treeNode::LAMBDA:
            // Create a new lambda closure Control object
            temp = new Control(Control::LAMBDA, variables, del_ptr, deltas_size - 1);
            break;
        case treeNode::INTEGER:
            // Create a new INTEGER Control object
            temp = new Control(Control::INTEGER, node->nodeString);
            break;
        case treeNode::MULTIPLY:
            // Create a new MULTIPLY Control object
            temp = new Control(Control::MULTIPLY, node->nodeString);
            break;
        case treeNode::ADD:
            // Create a new ADD Control object
            temp = new Control(Control::ADD, node->nodeString);
            break;
        case treeNode::SUBTRACT:
            // Create a new SUBTRACT Control object
            temp = new Control(Control::SUBTRACT, node->nodeString);
            break;
        case treeNode::DIVIDE:
            // Create a new DIVIDE Control object
            temp = new Control(Control::DIVIDE, node->nodeString);
            break;
        case treeNode::GAMMA:
            // Create a new GAMMA Control object
            temp = new Control(Control::GAMMA, node->nodeString);
            break;
        case treeNode::IDENTIFIER:
            // Create a new NAME Control object
            temp = new Control(node->nodeString, Control::NAME);
            break;
        case treeNode::STRING:
            // Create a new STRING Control object
            temp = new Control(Control::STRING, node->nodeString.substr(1, node->nodeString.length() - 2));
            break;
        case treeNode::TAU:
            if (variables != NULL)
                tau_count = variables->size();
            else
                cout << "TAU add_control NULL variables sent!" << endl;
            // Create a new TAU Control object
            temp = new Control(Control::TAU, tau_count, false);
            break;
        case treeNode::AUG:
            // Create a new AUG Control object
            temp = new Control(Control::AUG);
            break;
        case treeNode::NIL:
            // Create a new NIL Control object
            temp = new Control(Control::NIL);
            break;
        case treeNode::YSTAR:
            // Create a new YSTAR Control object
            temp = new Control(Control::YSTAR);
            break;
        case treeNode::AND_LOGICAL:
            // Create a new AND_LOGICAL Control object
            temp = new Control(Control::AND_LOGICAL);
            break;
        case treeNode::OR:
            // Create a new OR Control object
            temp = new Control(Control::OR);
            break;
        case treeNode::NE:
            // Create a new NE Control object
            temp = new Control(Control::NE);
            break;
        case treeNode::EQ:
            // Create a new EQ Control object
            temp = new Control(Control::EQ);
            break;
        case treeNode::LS:
            // Create a new LS Control object
            temp = new Control(Control::LS);
            break;
        case treeNode::LE:
            // Create a new LE Control object
            temp = new Control(Control::LE);
            break;
        case treeNode::GR:
            // Create a new GR Control object
            temp = new Control(Control::GR);
            break;
        case treeNode::GE:
            // Create a new GE Control object
            temp = new Control(Control::GE);
            break;
        case treeNode::NEG:
            // Create a new NEG Control object
            temp = new Control(Control::NEG);
            break;
        case treeNode::FALSE:
            // Create a new FALSE Control object
            temp = new Control(Control::FALSE);
            break;
        case treeNode::TRUE:
            // Create a new TRUE Control object
            temp = new Control(Control::TRUE);
            break;
        case treeNode::NOT:
            // Create a new NOT Control object
            temp = new Control(Control::NOT);
            break;
        case treeNode::DUMMY:
            // Create a new DUMMY Control object
            temp = new Control(Control::DUMMY);
            break;
        default:
            printf("Done. val: %s type: %d", value.c_str(), type);
            break;
    };
    ctrlStruct->push_back(temp);
}


