#include "CSE_Machine.h"

// Constructor for CSE_Machine with a topNode
CSE_Machine::CSE_Machine(treeNode* topNode) {
    this->top = topNode;
}

// Default constructor for CSE_Machine
CSE_Machine::CSE_Machine() {
    numEnvironment = -1;
    PE = createNewEnvironment();
    environmentStack.push(PE);
    deltas = new vector<Control*>;
    rootDelta = new Control(Control::DELTA, deltas->size());
}

// Destructor for CSE_Machine
CSE_Machine::~CSE_Machine() {
    // Clean up any resources or memory here
}

// Function to check if a function name is an inbuilt function
bool CSE_Machine::checkInbuilt(string funcName) {
    if (std::find(inbuiltFuncVector.begin(), inbuiltFuncVector.end(), funcName) != inbuiltFuncVector.end()) {
        return true;
    } else {
        return false;
    }
}

void CSE_Machine::applyBinaryOPR(int type){

    // Remove the last element from the 'control' container
    control.pop_back();

    // Get the top elements from the 'execStack'
    Control *rand1 = execStack.top() ;
    execStack.pop();
    Control *rand2 = execStack.top() ;
    execStack.pop();

    // Create a new 'Control' object called 'result'
    Control *result = new Control() ;
    result->type = Control::INTEGER;
    switch(type)
      {
      case Control::AUG :

    // If 'type' is 'Control::AUG', set 'result' type to 'Control::TUPLE'
    result->type = Control::TUPLE;
    result->ctrlTuples;

    // Process 'rand1' and 'rand2' objects
    if( rand1->type != Control::NIL ){
      if( rand1->type == Control::TUPLE){
        for(int i=0;i<rand1->ctrlTuples.size(); i++)
          result->ctrlTuples.push_back(rand1->ctrlTuples.at(i));
      }
      else{
        result->ctrlTuples.push_back(rand1) ;
      }
    }
    if( rand2->type != Control::NIL ){
      if( rand2->type == Control::TUPLE){
        for(int i=0;i<rand2->ctrlTuples.size(); i++)
          result->ctrlTuples.push_back(rand2->ctrlTuples.at(i));
      }
      else
        result->ctrlTuples.push_back(rand2);
    }
    break ;
      case Control::EXP :
    // If 'type' is 'Control::EXP', calculate the result of raising 'rand1' to the power of 'rand2'
    if(rand1->type == Control::INTEGER && rand2->type == Control::INTEGER) {
        // Calculate the result and store it in 'result->ctrlVal'
        result->ctrlVal = to_string(pow(atoi(rand1->ctrlVal.c_str()), atoi(rand2->ctrlVal.c_str())));
        break ;
      }
    else {
        // If none of the specified cases match, handle the default case
        cout << "Incompatible arguments for the operator '**'" ;
        exit(1) ;
      }
    break ;
      case Control::GR :
    // If both 'rand1' and 'rand2' are of type 'Control::INTEGER', compare their values
    if(rand1->type == Control::INTEGER && rand2->type == Control::INTEGER){
      if( atoi(rand1->ctrlVal.c_str()) > atoi(rand2->ctrlVal.c_str()) )
        result->type = Control::TRUE;
      else
        result->type = Control::FALSE;
      break ;
    }
    else{
      cout << "Incompatible arguments for the operator 'GR'" << endl ;
      exit(1) ;
    }
    break ;
      case Control::GE :
    // If both 'rand1' and 'rand2' are of type 'Control::INTEGER', compare their values
    if(rand1->type == Control::INTEGER && rand2->type == Control::INTEGER) {
        if( atoi(rand1->ctrlVal.c_str()) >= atoi(rand2->ctrlVal.c_str()) )
          result->type = Control::TRUE;
        else
          result->type = Control::FALSE;
        break ;
      }
    else {
        cout << "Incompatible arguments for the operator 'GE'" ;
        exit(1) ;
      }
    break ;
      case Control::LS :
    // If both 'rand1' and 'rand2' are of type 'Control::INTEGER', compare their values
    if(rand1->type == Control::INTEGER && rand2->type == Control::INTEGER) {
        if( atoi(rand1->ctrlVal.c_str()) < atoi(rand2->ctrlVal.c_str()) )
          result->type = Control::TRUE;
        else
          result->type = Control::FALSE;
        break ;
      }
    else {
        cout << "Incompatible arguments for the operator 'LS'" ;
        exit(1) ;
      }
    break ;
      case Control::LE :
    // If both 'rand1' and 'rand2' are of type 'Control::INTEGER', compare their values
    if(rand1->type == Control::INTEGER && rand2->type == Control::INTEGER) {
        if( atoi(rand1->ctrlVal.c_str()) <= atoi(rand2->ctrlVal.c_str()) )
          result->type = Control::TRUE;
        else
          result->type = Control::FALSE;
        break ;
      }
    else {
        cout << "Incompatible arguments for the operator 'LE'" ;
        exit(1) ;
      }
    break ;
      case Control::EQ :
    // Compare values based on the types of 'rand1' and 'rand2'
    if( rand1->type == Control::STRING && rand2->type == Control::STRING) {
        if( rand1->ctrlVal.compare(rand2->ctrlVal) == 0)
          result->type = Control::TRUE;
        else
          result->type = Control::FALSE;
                }
    else if( rand1->type == Control::INTEGER && rand2->type == Control::INTEGER )  {
        if( rand1->ctrlVal == rand2->ctrlVal )
          result->type = Control::TRUE;
        else
          result->type = Control::FALSE;
      }
    else if((rand1->type == Control::TRUE || rand1->type == Control::FALSE) && (rand1->type == Control::TRUE || rand2->type == Control::FALSE)){
        result->type = rand1->type == rand2->type?Control::TRUE: Control::FALSE;
      }
    else {
        cout << "Incompatible arguments for the operator 'EQ'" ;
        exit(1) ;
      }
    break ;
      case Control::NE :
    // Compare values based on the types of 'rand1' and 'rand2'
    if( rand1->type == Control::STRING && rand2->type == Control::STRING ) {
        if( rand1->ctrlVal.compare(rand2->ctrlVal) != 0)
          result->type = Control::TRUE;
        else
          result->type = Control::FALSE;
                }
    else if( rand1->type == Control::INTEGER && rand2->type == Control::INTEGER ) {
        // If both 'rand1' and 'rand2' are of type 'Control::INTEGER', compare their values
        if( rand1->ctrlVal != rand2->ctrlVal)
          result->type = Control::TRUE;
        else
          result->type = Control::FALSE;
      }
    else if((rand1->type == Control::TRUE || rand1->type == Control::FALSE) && (rand1->type == Control::TRUE || rand2->type == Control::FALSE)){
        // If 'rand1' and 'rand2' are of type 'Control::TRUE' or 'Control::FALSE', compare their types
        result->type = rand1->type != rand2->type?Control::TRUE: Control::FALSE;
      }
    else {
        cout << "Incompatible arguments for the operator 'NE'" ;
        exit(1) ;
      }
    break ;
      case Control::OR :
    // If both 'rand1' and 'rand2' are of type 'Control::TRUE' or 'Control::FALSE', perform logical OR
    if( (rand1->type == Control::TRUE || rand1->type == Control::FALSE ) &&
        (rand2->type == Control::TRUE || rand2->type == Control::FALSE) ) {
        bool op1 = (rand1->type == Control::TRUE )? true : false ;
        bool op2 = ( rand2->type == Control::TRUE )? true : false ;
        bool res = op1 | op2 ;
        result->type =res? Control::TRUE : Control::FALSE;
      }
    else {
        cout << "Incompatible arguments for the operator 'OR'" ;
        exit(1) ;
      }
    break ;
      case Control::AND_LOGICAL :
    // If both 'rand1' and 'rand2' are of type 'Control::TRUE' or 'Control::FALSE', perform logical AND
    if( (rand1->type == Control::TRUE || rand1->type == Control::FALSE ) &&
        (rand2->type == Control::TRUE || rand2->type == Control::FALSE) ) {
        bool op1 = (rand1->type == Control::TRUE)?true:false;
        bool op2 = (rand2->type == Control::TRUE)?true:false;
        bool res = op1 & op2;
        result->type = res? Control::TRUE : Control::FALSE;
      }
    else {
        cout << "Incompatible arguments for the operator 'AND'" ;
        exit(1) ;
      }
    break ;
      case Control::ADD :
    // If both 'rand1' and 'rand2' are of type 'Control::INTEGER', perform addition
    if(rand1->type == Control::INTEGER && rand2->type == Control::INTEGER){
        result->ctrlVal = to_string(atoi(rand1->ctrlVal.c_str()) + atoi(rand2->ctrlVal.c_str()));
        break ;
      }
    else{

        cout << "Incompatible arguments for the operator '+'" ;
        exit(1) ;
      }
    break ;

      case Control::SUBTRACT :
    // If both 'rand1' and 'rand2' are of type 'Control::INTEGER', perform subtraction
    if(rand1->type == Control::INTEGER && rand2->type == Control::INTEGER){
        result->ctrlVal = to_string(atoi(rand1->ctrlVal.c_str()) - atoi(rand2->ctrlVal.c_str()));
        break ;
      }
    else {
        cout << "Incompatible arguments for the operator '-'" ;
        exit(1) ;
      }
    break ;
      case Control::MULTIPLY :
    // If both 'rand1' and 'rand2' are of type 'Control::INTEGER', perform multiplication
    if(rand1->type == Control::INTEGER && rand2->type == Control::INTEGER){
        result->ctrlVal = to_string(atoi(rand1->ctrlVal.c_str()) * atoi(rand2->ctrlVal.c_str()));
        break ;
      }
    else{
        cout << "Incompatible arguments for the operator '*'" ;
        exit(1) ;
      }
    break ;
      case Control::DIVIDE :
    // If both 'rand1' and 'rand2' are of type 'Control::INTEGER', perform division
    if(rand1->type == Control::INTEGER && rand2->type == Control::INTEGER) {
        result->ctrlVal = to_string(atoi(rand1->ctrlVal.c_str()) / atoi(rand2->ctrlVal.c_str()));
        break ;
      }
    else {
        cout << "Incompatible arguments for the operator '*'" ;
        exit(1) ;
      }
    break ;
      default:
          cout << "UnHandled binary operator , type: " << type << endl;
      }

    execStack.push(result) ;

}

void CSE_Machine::escapePrintStr(string printStr){
// This function processes escape sequences in the 'printStr' and prints the result
  for( int i = 0 ; i < printStr.length() ; i++ ){
  char ch1 = printStr.at(i) ;
    if( ch1 == '\\'){
      i++ ;
      if( i < printStr.length() ){
        char ch2 = printStr.at(i) ;
        if( ch2 == 'n' )
      cout << endl;
        else if( ch2 == 't' )
      cout << "\t" ;
        else
      cout <<  ch1 << ch2 ;
      }
    }
    else
      cout << ch1 ;
    }
    cout.flush();
}

void CSE_Machine::applyThisRator(Control* rator){
    Control* temp;
    Control* toPush;
    if(rator->variables.front() == "Print"){
        // Handle the 'Print' operation
        string print_str = execStack.top()->toStr();
        escapePrintStr(print_str); 
        execStack.pop();
        toPush = new Control(Control::DUMMY);
    }else if(rator->variables.front() == "Order"){
        // Handle the 'Order' operation
        Control *tuple = execStack.top() ;
        execStack.pop();
        Control *order = new Control();
        order->type = Control::INTEGER;
        if( tuple->type == Control::TUPLE){
            order->ctrlVal = to_string(tuple->ctrlTuples.size());
        }else if( tuple->type == Control::NIL ){
            order->ctrlVal = to_string(0);
        }else{
            cout <<  "Invalid argument for 'Order'" << endl ;
            exit(1) ;
        }
        toPush = order;
    }else if(rator->variables.front() == "Isinteger"){
        // Check if the value is an integer
        temp = execStack.top();
        execStack.pop();
        toPush = new Control(temp->type == Control::INTEGER ? Control::TRUE : Control::FALSE);
    }else if(rator->variables.front() == "Istruthvalue"){
        // Check if the value is a truth value
        temp = execStack.top();
        execStack.pop();
        toPush = new Control((temp->type == Control::TRUE || temp->type == Control::FALSE) ? Control::TRUE : Control::FALSE);
    }else if(rator->variables.front() == "Isstring"){
        // Check if the value is a string
        temp = execStack.top();
        execStack.pop();
        toPush = new Control(temp->type == Control::STRING ? Control::TRUE : Control::FALSE);
    }else if(rator->variables.front() == "Istuple"){
        // Check if the value is a tuple or NIL
        temp = execStack.top();
        execStack.pop();
        toPush = new Control((temp->type == Control::TUPLE || temp->type == Control::NIL) ? Control::TRUE : Control::FALSE);
    }else if(rator->variables.front() == "Isfunction"){
        // Check if the value is a lambda function
        temp = execStack.top();
        execStack.pop();
        toPush = new Control(temp->type == Control::LAMBDA ? Control::TRUE : Control::FALSE);
    }else if(rator->variables.front() == "Isdummy"){
        // Check if the value is a dummy
        temp = execStack.top();
        execStack.pop();
        toPush = new Control(temp->type == Control::DUMMY ? Control::TRUE : Control::FALSE);
    }else if(rator->variables.front() == "Stem"){
        // Extract the first character from a string
        if(execStack.top()->type == Control::STRING){
            Control *strControl= new Control(Control::STRING);
            strControl->ctrlVal = execStack.top()->ctrlVal.substr(0, 1);
            execStack.pop();
            toPush = strControl;
        }else{
            cout << "STEM: Expecting String" << endl;
            exit(1);
        }
    }else if(rator->variables.front() == "Stern"){
        // Extract the substring excluding the first character from a string
        if(execStack.top()->type == Control::STRING){
            Control *strControl = new Control(Control::STRING);
            strControl->ctrlVal = execStack.top()->ctrlVal.substr(1, execStack.top()->ctrlVal.length()-1);
            execStack.pop();
            toPush = strControl;
        }else{
            cout << "STERN: Expecting String" << endl;
            exit(1);
        }
    }else if(rator->variables.front() == "ItoS"){
        // Convert an integer to a string
        if(execStack.top()->type == Control::INTEGER){
           Control *strControl = new Control(Control::STRING);
           strControl->ctrlVal = execStack.top()->ctrlVal;
           execStack.pop();
           toPush = strControl;
        }else{
            cout << "Itos: Expecting Integer" << endl;
            exit(1);
        }
    }else if(rator->variables.front() == "Conc"){ 
        // Handle the 'Conc' operation
        Control *concl = new Control(Control::NAME);
        concl->variables.push_back("Conclambda");
        concl->variables.push_back(execStack.top()->ctrlVal);
        execStack.pop();
        toPush = concl;
    }else if(rator->variables.front() == "Conclambda"){
        // Handle the 'Conclambda' operation
        Control *concatVars = new Control(Control::STRING, rator->variables.at(1)+ execStack.top()->ctrlVal);
        execStack.pop();
        toPush = concatVars;
    }else if(rator->variables.front() == "Null"){
        // Check if the value is NIL or an empty tuple
        Control *boolR = new Control();
        if(execStack.top()->type == Control::NIL || (execStack.top()->type == Control::TUPLE && execStack.top()->ctrlTuples.empty()))
            boolR->type = Control::TRUE;
        else
            boolR->type = Control::FALSE;
        execStack.pop();
        toPush = boolR;
    }else{
        cout << "ERROR: value:%" << rator->ctrlVal << "%type:" << rator->type << endl;
        printCS();
        return;
    }
    execStack.push(toPush);
}

void CSE_Machine::handleName(Control* temp, Control* currControl, Control* rator, Environment* newEnv, int deltaIndex){
    // Handle the name lookup in the current environment
    if(checkInbuilt(currControl->variables.front())){
        control.pop_back();
        execStack.push(currControl) ;
    } else {
        temp = currEnvironment->lookup(currControl->variables.front());
        if(temp != NULL){
            control.pop_back();
            execStack.push(temp) ;
        } else {
            cout << " Unknown name" +  currControl->ctrlVal << endl;
        }
    }
}

void CSE_Machine::rule411(Control* temp, Control* currControl, Control* rator, Environment* newEnv, int deltaIndex){
    // Apply rule 4.1.1: Create a new environment and handle variable assignments
    newEnv = createNewEnvironment();
    newEnv->assignParent(environmentMap.find(rator->associatedENV)->second);
    currEnvironment = newEnv ;
    if( rator->variables.size() == 1 ){
        // Single variable assignment
        currEnvironment->symbolTable[rator->variables.at(0)] = execStack.top();
        execStack.pop();
    } else {
        // Tuple assignment
        temp = execStack.top(); 
        execStack.pop() ;
        if( temp->type == Control::TUPLE && rator->variables.size() == temp->ctrlTuples.size() )
        {
            for( int i = 0 ; i < rator->variables.size() ; i++ )
            {
                currEnvironment->symbolTable[rator->variables.at(i)] = temp->ctrlTuples.at(i);
            }
        } else {
            cout << "Number/Type of arguments to a function doesn't match. rator variable size: "  << rator->variables.size() << " temp tuples->size:" << temp->ctrlTuples.size() << endl;
            exit(1);
        }
    }
    environmentStack.push(currEnvironment);
    control.push_back(new Control(Control::ENV, currEnvironment->id, false));
    execStack.push(new Control(Control::ENV, currEnvironment->id, false));
    for(int i=0;i<deltas->at(rator->index)->ctrlStruct->size();i++){
        control.push_back(deltas->at(rator->index)->ctrlStruct->at(i));
    }
}

void CSE_Machine::rule12(Control* temp, Control* currControl, Control* rator, Environment* newEnv, int deltaIndex){
    // Apply rule 12: Create an ETA control and push it onto the execution stack
    Control *eta = new Control(execStack.top()) ;
    execStack.pop();
    eta->type = Control::ETA;
    execStack.push(eta);
}

void CSE_Machine::rule13(Control* temp, Control* currControl, Control* rator, Environment* newEnv, int deltaIndex){
    // Apply rule 10: Index into a tuple
    control.push_back(new Control(Control::GAMMA));
    control.push_back(new Control(Control::GAMMA));
    execStack.push(rator);
    Control *lambda = new Control(Control::LAMBDA, &(rator->variables), NULL, rator->index) ;
    lambda->associatedENV = rator->associatedENV;
    execStack.push(lambda);
}

void CSE_Machine::rule10(Control* temp, Control* currControl, Control* rator, Environment* newEnv, int deltaIndex){
    // Apply rule 10: Index into a tuple
    if( execStack.top()->type == Control::INTEGER ){
    temp = rator->ctrlTuples.at(atoi(execStack.top()->ctrlVal.c_str()) - 1) ;
    execStack.pop();
    execStack.push(temp) ;
    } else{
        cout << "Expected an integer while indexing tuples!";
        exit(1) ;
    }
}


void CSE_Machine::handleGAMMA(Control* temp, Control* currControl, Control* rator, Environment* newEnv, int deltaIndex){
    // Handle the GAMMA operation
    control.pop_back();
    rator = execStack.top() ;
    execStack.pop();
    if( rator->type== Control::LAMBDA ){
        rule411(temp, currControl, rator, newEnv, deltaIndex);
    }else if( rator->type == Control::YSTAR ){
        rule12(temp, currControl, rator, newEnv, deltaIndex);
    } else if( rator->type == Control::ETA ){
        rule13(temp, currControl, rator, newEnv, deltaIndex);
    }else if(rator->type == Control::TUPLE){ //Rule 10
        rule10(temp, currControl, rator, newEnv, deltaIndex);
    } else {
        applyThisRator(rator);
    }
}


void CSE_Machine::handleBeta(Control* temp, Control* currControl, Control* rator, Environment* newEnv, int deltaIndex){
    // Handle the BETA operation
    control.pop_back();
    if(execStack.top()->type == Control::TRUE){
        control.pop_back();
        if(control.at(control.size() - 1)->type == Control::DELTA){
            deltaIndex = control.at(control.size() -1)->index;
            control.pop_back();
        }else{
            cout << "Delta Expected " << control.at(control.size() -1)->type << endl;
            exit(1);
        }
    }else if(execStack.top()->type == Control::FALSE){
        if(control.at(control.size() - 1)->type == Control::DELTA){
            deltaIndex = control.at(control.size() -1)->index;
            control.pop_back();
            control.pop_back();
        }else{
            cout << "Delta Expected, found, " << control.at(control.size() -1)->type << endl;
            exit(1);
        }
    }else{
        cout << " '->' operator Expected a bool value"  << endl;
        exit(1);
    }
    execStack.pop();
    for(int i=0; i< deltas->at(deltaIndex)->ctrlStruct->size() ; i++){
        control.push_back(deltas->at(deltaIndex)->ctrlStruct->at(i));
    }
}

void CSE_Machine::handleEnv(Control* temp, Control* currControl, Control* rator, Environment* newEnv, int deltaIndex){
    // Handle the ENV operation
    temp = execStack.top() ;
    execStack.pop();
    if( execStack.top()->type == Control::ENV && execStack.top()->index == currControl->index ){
        control.pop_back();
        execStack.pop();
        execStack.push(temp) ;

        environmentStack.pop() ;
        currEnvironment = environmentStack.top() ;
    }else{
        cout << "Environment markers do not match!" << endl;
        exit(1) ;
    }
}

void CSE_Machine::handleTau(Control* temp, Control* currControl, Control* rator, Environment* newEnv, int deltaIndex){
    // Handle the TAU operation
    control.pop_back();
    temp = new Control() ;
    temp->type = Control::TUPLE;
    temp->ctrlTuples.clear();
    for(int i=0; i<currControl->index; i++ )
    {
        temp->ctrlTuples.push_back(execStack.top()) ;
        execStack.pop() ;
    }
    execStack.push(temp) ;

}

void CSE_Machine::handleNeg(Control* temp, Control* currControl, Control* rator, Environment* newEnv, int deltaIndex){
    // Handle the NEG operation
    control.pop_back();
    if(execStack.top()->type == Control::INTEGER )
        execStack.top()->ctrlVal = to_string(-1*atoi(execStack.top()->ctrlVal.c_str()));
    else
    {
        cout << "Neg: Int not found" << endl;
        exit(1) ;
    }
}

void CSE_Machine::execCSE(){
    Control *temp = NULL;
    Control *currControl = NULL;
    Control *rator = NULL;
    Environment *newEnv = NULL;
    int deltaIndex = -1;
    while(!control.empty()){
        //printCS();
        currControl = control.at(control.size()-1);
        temp = NULL;
        switch(currControl->type){
        case Control::INTEGER :
        case Control::STRING :
        case Control::TRUE :
        case Control::FALSE :
        case Control::DUMMY :
        case Control::YSTAR :
            // Push constants onto the execution stack
            control.pop_back();
            execStack.push(currControl) ;
            break;
        case Control::NAME:
            // Handle name lookup
            handleName(temp, currControl, rator, newEnv, deltaIndex);
            break;
        case Control::NIL :
            // Create an empty tuple
            currControl->ctrlTuples.clear();
            control.pop_back();
            execStack.push(currControl) ;
            break ;
        case Control::LAMBDA :
            // Apply CSE Rule 2: Set associated environment for lambda
            currControl->associatedENV = currEnvironment->id;
            control.pop_back();
            execStack.push(currControl) ;
            break ;
        case Control::GAMMA:
            // Handle GAMMA operation
            handleGAMMA(temp, currControl, rator, newEnv, deltaIndex);
            break;
        case Control::NOT :
            // Apply NOT operation
            control.pop_back();
            temp = execStack.top() ;
            if( temp->type == Control::TRUE || temp->type == Control::FALSE ){
                temp->type =  temp->type ==  Control::TRUE ? Control::FALSE : Control::TRUE;
            }else{
                cout << "Expecting a bool value for operator 'not'" << endl;
                exit(1) ;
            }
            break ;
        
        case Control::GR :
        case Control::GE :
        case Control::LS :
        case Control::LE :
        case Control::EQ :
        case Control::NE :
        case Control::AUG :
        case Control::OR :
        case Control::AND_LOGICAL :
        case Control::ADD :
        case Control::SUBTRACT :
        case Control::MULTIPLY :
        case Control::DIVIDE :
        case Control::EXP :
            applyBinaryOPR(currControl->type) ;
            break ;
        case Control::TAU:
            handleTau(temp, currControl, rator, newEnv, deltaIndex);
            break ;
        case Control::NEG : 
            handleNeg(temp, currControl, rator, newEnv, deltaIndex);
            break ;
        case Control::ENV:
            handleEnv(temp, currControl, rator, newEnv, deltaIndex);
            break;
        case Control::BETA:
            handleBeta(temp, currControl, rator, newEnv, deltaIndex);
            break;
        default:
            cout << "Unknown Control Type: " << currControl->type << endl;
            break;
        }
    }
}

Environment* CSE_Machine::createNewEnvironment(){
    // Create a new environment with a unique ID
    tempEnvironment = new Environment(numEnvironment);
    environmentMap[numEnvironment] =  tempEnvironment;
    numEnvironment++;
    return tempEnvironment;
}

void CSE_Machine::printCS(){
    // Print the contents of the control stack
    for(int i=0; i < control.size() ; i++){
        cout << control.at(i)->toStr() << " " ;
    }
    printf ("---------");

    // Print the contents of the execution stack
    stack<Control *> temp;
    while(!execStack.empty()){
        cout << execStack.top()->toStr() << " " ;
        temp.push(execStack.top());
        execStack.pop();
    }
    printf ("\n");

    // Restore the original order of the execution stack
    while(!temp.empty()){
      execStack.push(temp.top());
      temp.pop();
    }
}

void CSE_Machine::deltaPrint(){
    // Print the contents of each delta
    for(int j = 0; j < deltas->size(); j++){
        cout << deltas->at(j)->toStr() << " : ";
        for(int k = 0; k < deltas->at(j)->ctrlStruct->size(); k++){
            cout << deltas->at(j)->ctrlStruct->at(k)->toStr() << " ";
        }
        cout << endl;
    }
}

void CSE_Machine::init(treeNode *root){
    // Initialize the CSE machine
    deltas->push_back(rootDelta);
    flattenTree(root,rootDelta, deltas);
    control.push_back(new Control(Control::ENV, 0, false));
    execStack.push(new Control(Control::ENV, 0, false));
    for(int i=0; i< rootDelta->ctrlStruct->size(); i++)
        control.push_back(rootDelta->ctrlStruct->at(i));
    environmentStack.push(createNewEnvironment());
    environmentStack.top()->assignParent(PE);
    currEnvironment = environmentStack.top();
}

void CSE_Machine::run(treeNode *root){
    // Execute the CSE machine
    init(root);
    execCSE();
    if(!control.empty() || environmentStack.size() != 1){
        printf("stack/env_stack is not empty");
    }
    printf ("\n");
}

void CSE_Machine::flattenDeltaThen(treeNode* node, Control *delta,vector<Control *> *deltas){
    // Create a new delta for the "then" branch
    Control *deltaThen = new Control(Control::DELTA, deltas->size());
    deltas->push_back(deltaThen);
    delta->ctrlStruct->push_back(new Control(Control::DELTA, deltas->size()-1)); 
    if(node->childNode->siblingNode->type == treeNode::TERNARY){
        // Recurse into the "then" branch
        flattenTree(node->childNode->siblingNode, deltaThen, deltas);
    }else{
        // Handle non-ternary case
        vector<string> *tempvariables = NULL;
        if(node->childNode->siblingNode->type == treeNode::TAU){
            treeNode *temp = node->childNode->siblingNode->childNode;
            tempvariables = new vector<string>;
            while(temp!= NULL){
                tempvariables->push_back(temp->nodeString); 
                temp = temp->siblingNode;
            }
        }
        deltaThen->addCtrl(node->childNode->siblingNode, node->childNode->siblingNode->type, node->childNode->siblingNode->nodeString, tempvariables, deltaThen, deltas->size());
        if(node->childNode->siblingNode->childNode != NULL)
            flattenTree(node->childNode->siblingNode->childNode, deltaThen, deltas);
    }
}

void CSE_Machine::flattenDeltaElse(treeNode* node, Control *delta,vector<Control *> *deltas){
    // Create a new delta for the "else" branch
    Control *deltaElse = new Control(Control::DELTA, deltas->size());
    deltas->push_back(deltaElse);
    delta->ctrlStruct->push_back(new Control(Control::DELTA, deltas->size()-1));

    if(node->childNode->siblingNode->siblingNode->type == treeNode::TERNARY){
        // Recurse into the "else" branch
        flattenTree(node->childNode->siblingNode->siblingNode,deltaElse, deltas);
    }else{
        // Handle non-ternary case
        vector<string> *tempvariables = NULL;
        if(node->childNode->siblingNode->siblingNode->type == treeNode::TAU){
            treeNode *temp = node->childNode->siblingNode->siblingNode->childNode;
            tempvariables = new vector<string>;
            while(temp!= NULL){
                tempvariables->push_back(temp->nodeString);
                temp = temp->siblingNode;
            }
        }
        deltaElse->addCtrl(node->childNode->siblingNode->siblingNode, node->childNode->siblingNode->siblingNode->type, node->childNode->siblingNode->siblingNode->nodeString, tempvariables, deltaElse, deltas->size());
        if(node->childNode->siblingNode->siblingNode->childNode != NULL)
            flattenTree(node->childNode->siblingNode->siblingNode->childNode,deltaElse, deltas);
    }
}

void CSE_Machine::flattenTernary(treeNode* node, Control *delta,vector<Control *> *deltas){
    // Flatten the "then" branch
    flattenDeltaThen(node, delta, deltas);

    // Flatten the "else" branch
    flattenDeltaElse(node, delta, deltas);

    // Create a BETA control for the ternary expression
    Control *beta = new Control(Control::BETA);
    delta->ctrlStruct->push_back(new Control(Control::BETA, "beta"));
    delta->addCtrl(node->childNode, node->childNode->type, node->childNode->nodeString, NULL, NULL, deltas->size());
    if(node->childNode->childNode != NULL)
        flattenTree(node->childNode->childNode, delta, deltas);
}

void CSE_Machine::flattenLAMBDA(treeNode* node, Control *delta,vector<Control *> *deltas){
    // Flattens the tree structure for the LAMBDA operation
    Control *temp = NULL;
    vector<string> *variables = NULL;
    variables = new vector<string>();
    if(treeNode::IDENTIFIER == node->childNode->type){
        variables->push_back(node->childNode->nodeString);
    }else if(treeNode::COMMA == node->childNode->type){
        treeNode *temp = node->childNode->childNode;
        while(NULL != temp){
            variables->push_back(temp->nodeString);
            temp = temp->siblingNode;
        }
    }else{
        cout << "Expected Identifier or Comma, but din't find" << endl;
    }

    // Create a new delta for the LAMBDA expression
    temp = new Control(Control::DELTA, deltas->size());
    deltas->push_back(temp);
    delta->addCtrl(node, node->type, node->nodeString, variables, temp, deltas->size());

    // Flatten the LAMBDA body
    flattenTree(node->childNode->siblingNode, temp, deltas);

    if(NULL != node->siblingNode)
        flattenTree(node->siblingNode,delta, deltas);

}

void CSE_Machine::flattenTree(treeNode* node, Control *delta,vector<Control *> *deltas){
    Control *temp_del_ptr = NULL;
    vector<string> *variables = NULL;
    if(treeNode::LAMBDA == node->type){
        // Handle LAMBDA node
        flattenLAMBDA(node, delta, deltas);
    }else if(node->type == treeNode::TERNARY){
        // Handle TERNARY node
        flattenTernary(node, delta, deltas);
    }else{
        if(node->type == treeNode::TAU){
            // Extract variables for TAU node
            variables = new vector<string>();
            treeNode *temp = node->childNode;
            while(temp!= NULL){
                variables->push_back(temp->nodeString);
                temp = temp->siblingNode;
            }
        }
        // Add control for the current node
        delta->addCtrl(node, node->type, node->nodeString, variables, temp_del_ptr, deltas->size());

        // Recurse into child and sibling nodes
        if(NULL != node->childNode){
            flattenTree(node->childNode, delta, deltas);
        }
        if(NULL != node->siblingNode){
            flattenTree(node->siblingNode, delta,deltas);
        }
    }
}
