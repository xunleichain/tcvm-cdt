#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"

#include "llvm/Support/CommandLine.h"

#include "minijson_writer.hpp"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <string>
#include <sstream>
#include <vector>
#include <fstream>

using namespace clang;
using namespace std;
using namespace clang::tooling;
using namespace llvm;
using namespace clang::ast_matchers;


enum TCError{
	None,
	UnFormatContract,
};

TCError error = TCError::None;
bool foundContract = false;

struct Input{
    std::string name;
    std::string type;
    std::vector<Input> components;
};

typedef Input Output;

struct Method{
    std::string name;
    std::vector<Input> inputs;
    std::vector<Output> outputs;
};

std::vector<std::string> payfalseMethod;

namespace minijson
{
template<>
struct default_value_writer<Input>
{
    void operator()(std::ostream& stream, const Input& input, const writer_configuration& configuration) const{
        minijson::object_writer writer(stream, configuration);
        if (!input.name.empty()){
            writer.write("name", input.name);
        }
        writer.write("type", input.type);
        if (input.components.size() > 0) {
            writer.write_array("components", input.components.begin(), input.components.end());
        }
        writer.close();
    }
};

template<>
struct default_value_writer<Method>
{
    void operator()(std::ostream& stream, const Method& method, const writer_configuration& configuration) const{
        minijson::object_writer writer(stream, configuration);
        writer.write("name", method.name);
		if (std::find(payfalseMethod.begin(), payfalseMethod.end(), method.name) == payfalseMethod.end()){
			writer.write("payable", true);// method.name);
		}else {
			writer.write("payable", false);// method.name);
		}
        writer.write_array("inputs", method.inputs.begin(), method.inputs.end());
        writer.write_array("outputs", method.outputs.begin(), method.outputs.end());
        writer.close();
    }
};
}

class JsonNode{

public:
    JsonNode(){}

    void addMethod(const CXXMethodDecl* decl){
        //add method object
        Method method;
        method.name = decl->getNameAsString();

        //add param vector
        for (auto param : decl->parameters() ) {
            Input input;
            addArg(param, input);
            method.inputs.push_back(input);
        }

        //llvm::outs()<<decl->getReturnType().getAsString()<<"\n";
        addReturnType(decl->getReturnType(), method.outputs);

        methods.push_back(method);
       /* 
        llvm::outs()<<"built-in:"<<decl->getReturnType()->isBuiltinType()<<"\n";

        //void
        llvm::outs()<<"Fundamental:"<<decl->getReturnType()->isFundamentalType()<<"\n";

        //tuple
        llvm::outs()<<"struct:"<<decl->getReturnType()->isStructureOrClassType()<<"\n";

        //tuple
        llvm::outs()<<"Depend:"<<decl->getReturnType()->isDependentType()<<"\n";

        //person
        llvm::outs()<<"InstantiationDepend:"<<decl->getReturnType()->isInstantiationDependentType()<<"\n";


        llvm::outs()<<"TemplateTypeParm:"<<decl->getReturnType()->isTemplateTypeParmType()<<"\n";

        llvm::outs()<<"InstantiationDepend"<<decl->getReturnType()->isInstantiationDependentType()<<"\n";
        
        if (decl->getReturnType()->getAs<TemplateSpecializationType>() == nullptr){
            llvm::outs()<<"Convert templateType false"<<"\n";
        }
        
        if (decl->getReturnType()->getAsRecordDecl() == nullptr){
            llvm::outs()<<"Convert RecordDecl false"<<"\n";
        }
        */

        //output type:
        //1.struct 2.build-in 3.tuple-template

        //auto par_type = param->getType().split().asPair().first->getAsRecordDecl();
        //TODO type->CXXRecordDecl->TemplateSpecializationDecl->ClassTemplateSpecializationDecl->TemplateArgument
        //add output
        //llvm::outs()<<"method:"<<method.name<<"\n";
    }

    void addReturnType(const QualType& retType, std::vector<Output>& outputs){

        if (retType->getAs<TemplateSpecializationType>() == nullptr){
            addNonTupleType(retType, outputs); 

        } else {
            auto templateType = retType->getAs<TemplateSpecializationType>();
            if (templateType->getTemplateName().getAsTemplateDecl()->getNameAsString() != "tuple"){
                addNonTupleType(retType, outputs); 
                return;
            }

            for (auto iter = templateType->begin() ; iter != templateType->end(); iter++){
                Output output;
                output.type = iter->getAsType().getAsString();
                if (iter->getAsType().split().asPair().first->isStructureOrClassType()){
                    auto par_type = iter->getAsType().split().asPair().first->getAsRecordDecl();
                    addComposition(par_type, output.components);
                } 
                outputs.push_back(output);
            }
        }
    }

    void addNonTupleType(const QualType& retType, std::vector<Output>& outputs){
        if (retType->getAsRecordDecl() != nullptr){
            Output output;
            output.type = retType.getAsString();
            addComposition(retType->getAsRecordDecl(), output.components);
            outputs.push_back(output);
        } else {
            Output output;
            output.type = retType.getAsString();
            outputs.push_back(output);
        }
    }

    //inputs:[{"name": "", "type":""}, ... ]
    void addArg(const ParmVarDecl* param, Input& input){

    //Type arg ...
        input.name = param->getNameAsString();
        input.type = param->getOriginalType().getAsString();

    //llvm::outs()<<"input:"<<input.name<<"\n";
        if (param->getType().split().asPair().first->isStructureOrClassType()){
            auto par_type = param->getType().split().asPair().first->getAsRecordDecl();
            addComposition(par_type, input.components);
        }
    }

    void addComposition(const RecordDecl* decl, std::vector<Input>& components){
        for (auto field : decl->fields()) {
            Input input;
            input.name = field->getName().str();
            input.type = field->getType().getAsString();

            if (field->getType().split().asPair().first->isStructureOrClassType()){
                auto par_type = field->getType().split().asPair().first->getAsRecordDecl();
                addComposition(par_type, input.components);
            }
   
            components.push_back(input);
        //  llvm::outs()<<field->getType().str();
        }
    }

    string toString(){
        if (error == TCError::None && foundContract){
            std::stringstream stream;
            minijson::write_array(stream, methods.begin(), methods.end());
            return stream.str();
        } else {
            return string("[]"); 
        }
    }

    void printInput(Input& input){
        if (!input.name.empty()){
            llvm::outs()<<"input.name: "<<input.name<<"\n";
        }
        llvm::outs()<<"input.type: "<<input.type<<"\n";
        if (input.components.size() > 0){
            llvm::outs()<<"component: "<<"\n";
        }
        for (auto component : input.components){
            printInput(component);
        }
    }

    void toStringTest(){
        for(auto method : methods){
            llvm::outs()<<"methods: "<<method.name<<"\n";
            for (auto input : method.inputs){
                printInput(input);
            }
            llvm::outs()<<"\n";
        }
    }

private:
    std::string contractName;
    std::vector<Method> methods;

};

static JsonNode* getJsonNode(){
    static JsonNode jsonNode;
    return &jsonNode;
}

DeclarationMatcher ContractMatcher = cxxRecordDecl(isDerivedFrom(hasName("TCBaseContract"))).bind("contract");

class TransToJson : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result)
   	{
		//llvm::outs()<<"Find!!\n";
        CXXRecordDecl const * Declaration = Result.Nodes.getNodeAs<CXXRecordDecl>("contract");
		//Declaration->PrintStats();
		for (auto iter = Declaration->bases_begin();iter != Declaration->bases_end();iter++){
			//llvm::outs()<<iter->getType().getAsString()<<"\n";
            if (iter->getType().getAsString()== "class TCBaseContract"){
                if (foundContract){
                    error = UnFormatContract;		
					break;
                }
                foundContract = true;
                for ( auto iter = Declaration->method_begin();iter != Declaration->method_end();iter++){
                    
                    if (iter->getAccess() != AccessSpecifier::AS_public){
                        continue;
                    }
					if (iter->isCopyAssignmentOperator()){
                        continue;
					}
					if (iter->isMoveAssignmentOperator()){
                        continue;
					}
					if (iter->isUserProvided()){
						getJsonNode()->addMethod(*iter);
                        
					}
                }
            }
        }
	}
};

DeclarationMatcher MethodMatcher = cxxMethodDecl(ofClass(isDerivedFrom("TCBaseContract")), 
		hasDescendant(
			callExpr( callee(functionDecl(hasName("TC_Payable"))) 
					  ,hasArgument(0, cxxBoolLiteral(equals(false)))
					)
			 )
).bind("payable_method");

class PayableMatcher : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) {
        CXXMethodDecl const * method = Result.Nodes.getNodeAs<CXXMethodDecl>("payable_method");
		payfalseMethod.push_back(method->getNameAsString());
//		llvm::outs()<<"pay method:"<<method->getNameAsString()<<"\n";
	}
};

static cl::OptionCategory TCToolCategory("ThunderChain Tool options");
#define INCLUDE_FILE_NUM (8)

int main(int argc, const char **argv) {

	//include file -I
	std::vector<std::string> file_options;
	for (size_t i = 0; i < argc - INCLUDE_FILE_NUM; i++) {
		file_options.push_back(argv[i]);
	}

	std::vector<std::string> include_options;
	for (size_t i=argc - INCLUDE_FILE_NUM; i < argc; i++) {
		include_options.push_back(argv[i]);
	}
	
	std::vector<std::string> options;
	for (auto& i:file_options){
		options.push_back(i);
	}
	
	options.push_back("--");
	options.push_back("--target=wasm32");
	options.push_back("-nostdlib");
	options.push_back("-ffreestanding");
	options.push_back("-fno-builtin");
	options.push_back("-fno-rtti");
	options.push_back("-fno-exceptions");
	options.push_back("-DBOOST_DISABLE_ASSERTS");
	options.push_back("-DBOOST_EXCEPTION_DISABLE");
	options.push_back("-Wno-everything");
	options.push_back("-fsyntax-only");
	options.push_back("-std=c++17");

	for (auto& i:include_options){
		options.push_back(i);
	}
	int size = options.size();
	const char** new_argv = new const char*[size];

	for (size_t i=0; i < size; i++){
		new_argv[i] = options[i].c_str();

	//	std::cout<<"argc:"<<i<<" "<<new_argv[i]<<std::endl;
	}

	CommonOptionsParser OptionsParser(size, new_argv, TCToolCategory);

	ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

	TransToJson Transer;
	PayableMatcher payableMatcher;
	MatchFinder Finder;

//	TransToJson Transer;

	Finder.addMatcher(MethodMatcher, &payableMatcher);
	Finder.addMatcher(ContractMatcher, &Transer);

	Tool.run(newFrontendActionFactory(&Finder).get());

	std::ofstream output("output.abi");
	output<<getJsonNode()->toString();
//	getJsonNode()->toStringTest();
	output.close();
}
