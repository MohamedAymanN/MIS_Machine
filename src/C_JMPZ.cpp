#include "C_JMPZ.h"

C_JMPZ::C_JMPZ(MIS *mis, int &P_PC)
{
    if (mis != nullptr && P_PC != -1) {
        misInst = mis;
        V_PC = &P_PC;
    }
}
C_Instruction * C_JMPZ::F_Clone(MIS *mis, int &P_PC){
    C_JMPZ * jmpz = new C_JMPZ(mis, P_PC);
    return jmpz;
}
/** Checks all syntax errors and calls ReportError if theres an error**/
void C_JMPZ::F_CheckSyntax(){
    /** Loading instruction string vector into params**/
    params = misInst->F_GetMainParser()->LinesInstrcts[(*V_PC)];
    /**Checking Regex for the desired format (for each parameter)**/
    regex V_test("(\\$[A-Za-z][A-Za-z0-9]{0,255}|([-]?([0-9]*[.])?[0-9]+))");
    if((*params).size() != 4 || !misInst->F_LabelExists((*params)[2])){
        V_ErrorMessage = "In function JMPZ: Too many arguments passed to JMPZ or label does not exist.";
        V_ErrorLine = (*V_PC);
        F_ReportError();
        return;
    }
    /** parameter is a variable that does not exist in MUMERIC and REAL maps **/
    if (!regex_match((*params)[3],V_test) |
        (*params)[3][0] == '$' && (!misInst->F_NumericVExists((*params)[3]) && !misInst->F_RealVExists((*params)[3]))){
        V_ErrorMessage = "In function JMPZ: Syntax does not match or variable does not exist.";
        V_ErrorLine = (*V_PC);
        F_ReportError();
        return;
    }
}

/** Checks if the condition is true to jump or not **/
void C_JMPZ::F_Execute(){
    /** Loading instruction string vector into params**/
    params = misInst->F_GetMainParser()->LinesInstrcts[(*V_PC)];
        /** Parameter is a variable **/
        if ((*params)[3][0] == '$'){
            string varName = (*params)[3];
            /** parameter is a NUMERIC variable **/
            if(misInst->F_NumericVExists(varName)){
                /** Check its a zero **/
                if(misInst->F_GetNumericV(varName) == 0){
                    (*V_PC) = (misInst->F_LabelLine((*params)[2]));
                    return;
                }
            }
            /** parameter is a REAL variable **/
            else if(misInst->F_RealVExists((*params)[3])){
                 /** Check its a zero **/
                 if(misInst->F_GetRealV(varName) == 0){
                    (*V_PC) = (misInst->F_LabelLine((*params)[2]));
                    return;
                 }
            }
        }
        /** parameter is not a variable **/
        else{
            /** Check its a zero **/
            if(stod((*params)[3]) == 0){
                (*V_PC) = (misInst->F_LabelLine((*params)[2]));
                return;
            }
    }
}

C_JMPZ::~C_JMPZ(){}

#ifdef JMPZGUARD
    extern "C" C_Instruction * F_GetInstruction() {return new C_JMPZ(nullptr);}
#endif
