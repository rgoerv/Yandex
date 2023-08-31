
// Generated from Formula.g4 by ANTLR 4.13.0


#include "FormulaLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct FormulaLexerStaticData final {
  FormulaLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  FormulaLexerStaticData(const FormulaLexerStaticData&) = delete;
  FormulaLexerStaticData(FormulaLexerStaticData&&) = delete;
  FormulaLexerStaticData& operator=(const FormulaLexerStaticData&) = delete;
  FormulaLexerStaticData& operator=(FormulaLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag formulalexerLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
FormulaLexerStaticData *formulalexerLexerStaticData = nullptr;

void formulalexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (formulalexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(formulalexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<FormulaLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "INT", "UINT", "EXPONENT", "NUMBER", "ADD", "SUB", 
      "MUL", "DIV", "WS"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'('", "')'", "", "'+'", "'-'", "'*'", "'/'"
    },
    std::vector<std::string>{
      "", "", "", "NUMBER", "ADD", "SUB", "MUL", "DIV", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,8,69,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,
  	2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,1,0,1,0,1,1,1,1,1,2,3,2,29,8,2,1,2,
  	1,2,1,3,4,3,34,8,3,11,3,12,3,35,1,4,1,4,1,4,1,5,1,5,3,5,43,8,5,1,5,3,
  	5,46,8,5,1,5,1,5,1,5,3,5,51,8,5,3,5,53,8,5,1,6,1,6,1,7,1,7,1,8,1,8,1,
  	9,1,9,1,10,4,10,64,8,10,11,10,12,10,65,1,10,1,10,0,0,11,1,1,3,2,5,0,7,
  	0,9,0,11,3,13,4,15,5,17,6,19,7,21,8,1,0,4,2,0,43,43,45,45,1,0,48,57,2,
  	0,69,69,101,101,3,0,9,10,13,13,32,32,72,0,1,1,0,0,0,0,3,1,0,0,0,0,11,
  	1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,
  	0,0,1,23,1,0,0,0,3,25,1,0,0,0,5,28,1,0,0,0,7,33,1,0,0,0,9,37,1,0,0,0,
  	11,52,1,0,0,0,13,54,1,0,0,0,15,56,1,0,0,0,17,58,1,0,0,0,19,60,1,0,0,0,
  	21,63,1,0,0,0,23,24,5,40,0,0,24,2,1,0,0,0,25,26,5,41,0,0,26,4,1,0,0,0,
  	27,29,7,0,0,0,28,27,1,0,0,0,28,29,1,0,0,0,29,30,1,0,0,0,30,31,3,7,3,0,
  	31,6,1,0,0,0,32,34,7,1,0,0,33,32,1,0,0,0,34,35,1,0,0,0,35,33,1,0,0,0,
  	35,36,1,0,0,0,36,8,1,0,0,0,37,38,7,2,0,0,38,39,3,5,2,0,39,10,1,0,0,0,
  	40,42,3,7,3,0,41,43,3,9,4,0,42,41,1,0,0,0,42,43,1,0,0,0,43,53,1,0,0,0,
  	44,46,3,7,3,0,45,44,1,0,0,0,45,46,1,0,0,0,46,47,1,0,0,0,47,48,5,46,0,
  	0,48,50,3,7,3,0,49,51,3,9,4,0,50,49,1,0,0,0,50,51,1,0,0,0,51,53,1,0,0,
  	0,52,40,1,0,0,0,52,45,1,0,0,0,53,12,1,0,0,0,54,55,5,43,0,0,55,14,1,0,
  	0,0,56,57,5,45,0,0,57,16,1,0,0,0,58,59,5,42,0,0,59,18,1,0,0,0,60,61,5,
  	47,0,0,61,20,1,0,0,0,62,64,7,3,0,0,63,62,1,0,0,0,64,65,1,0,0,0,65,63,
  	1,0,0,0,65,66,1,0,0,0,66,67,1,0,0,0,67,68,6,10,0,0,68,22,1,0,0,0,8,0,
  	28,35,42,45,50,52,65,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  formulalexerLexerStaticData = staticData.release();
}

}

FormulaLexer::FormulaLexer(CharStream *input) : Lexer(input) {
  FormulaLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *formulalexerLexerStaticData->atn, formulalexerLexerStaticData->decisionToDFA, formulalexerLexerStaticData->sharedContextCache);
}

FormulaLexer::~FormulaLexer() {
  delete _interpreter;
}

std::string FormulaLexer::getGrammarFileName() const {
  return "Formula.g4";
}

const std::vector<std::string>& FormulaLexer::getRuleNames() const {
  return formulalexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& FormulaLexer::getChannelNames() const {
  return formulalexerLexerStaticData->channelNames;
}

const std::vector<std::string>& FormulaLexer::getModeNames() const {
  return formulalexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& FormulaLexer::getVocabulary() const {
  return formulalexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView FormulaLexer::getSerializedATN() const {
  return formulalexerLexerStaticData->serializedATN;
}

const atn::ATN& FormulaLexer::getATN() const {
  return *formulalexerLexerStaticData->atn;
}




void FormulaLexer::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  formulalexerLexerInitialize();
#else
  ::antlr4::internal::call_once(formulalexerLexerOnceFlag, formulalexerLexerInitialize);
#endif
}
