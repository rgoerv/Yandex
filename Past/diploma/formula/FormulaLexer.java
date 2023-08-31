// Generated from Formula.g4 by ANTLR 4.13.0
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue"})
public class FormulaLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.13.0", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, NUMBER=3, ADD=4, SUB=5, MUL=6, DIV=7, WS=8;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"T__0", "T__1", "INT", "UINT", "EXPONENT", "NUMBER", "ADD", "SUB", "MUL", 
			"DIV", "WS"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'('", "')'", null, "'+'", "'-'", "'*'", "'/'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, "NUMBER", "ADD", "SUB", "MUL", "DIV", "WS"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}


	public FormulaLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "Formula.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\u0004\u0000\bE\u0006\uffff\uffff\u0002\u0000\u0007\u0000\u0002\u0001"+
		"\u0007\u0001\u0002\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004"+
		"\u0007\u0004\u0002\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007"+
		"\u0007\u0007\u0002\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0001\u0000"+
		"\u0001\u0000\u0001\u0001\u0001\u0001\u0001\u0002\u0003\u0002\u001d\b\u0002"+
		"\u0001\u0002\u0001\u0002\u0001\u0003\u0004\u0003\"\b\u0003\u000b\u0003"+
		"\f\u0003#\u0001\u0004\u0001\u0004\u0001\u0004\u0001\u0005\u0001\u0005"+
		"\u0003\u0005+\b\u0005\u0001\u0005\u0003\u0005.\b\u0005\u0001\u0005\u0001"+
		"\u0005\u0001\u0005\u0003\u00053\b\u0005\u0003\u00055\b\u0005\u0001\u0006"+
		"\u0001\u0006\u0001\u0007\u0001\u0007\u0001\b\u0001\b\u0001\t\u0001\t\u0001"+
		"\n\u0004\n@\b\n\u000b\n\f\nA\u0001\n\u0001\n\u0000\u0000\u000b\u0001\u0001"+
		"\u0003\u0002\u0005\u0000\u0007\u0000\t\u0000\u000b\u0003\r\u0004\u000f"+
		"\u0005\u0011\u0006\u0013\u0007\u0015\b\u0001\u0000\u0004\u0002\u0000+"+
		"+--\u0001\u000009\u0002\u0000EEee\u0003\u0000\t\n\r\r  H\u0000\u0001\u0001"+
		"\u0000\u0000\u0000\u0000\u0003\u0001\u0000\u0000\u0000\u0000\u000b\u0001"+
		"\u0000\u0000\u0000\u0000\r\u0001\u0000\u0000\u0000\u0000\u000f\u0001\u0000"+
		"\u0000\u0000\u0000\u0011\u0001\u0000\u0000\u0000\u0000\u0013\u0001\u0000"+
		"\u0000\u0000\u0000\u0015\u0001\u0000\u0000\u0000\u0001\u0017\u0001\u0000"+
		"\u0000\u0000\u0003\u0019\u0001\u0000\u0000\u0000\u0005\u001c\u0001\u0000"+
		"\u0000\u0000\u0007!\u0001\u0000\u0000\u0000\t%\u0001\u0000\u0000\u0000"+
		"\u000b4\u0001\u0000\u0000\u0000\r6\u0001\u0000\u0000\u0000\u000f8\u0001"+
		"\u0000\u0000\u0000\u0011:\u0001\u0000\u0000\u0000\u0013<\u0001\u0000\u0000"+
		"\u0000\u0015?\u0001\u0000\u0000\u0000\u0017\u0018\u0005(\u0000\u0000\u0018"+
		"\u0002\u0001\u0000\u0000\u0000\u0019\u001a\u0005)\u0000\u0000\u001a\u0004"+
		"\u0001\u0000\u0000\u0000\u001b\u001d\u0007\u0000\u0000\u0000\u001c\u001b"+
		"\u0001\u0000\u0000\u0000\u001c\u001d\u0001\u0000\u0000\u0000\u001d\u001e"+
		"\u0001\u0000\u0000\u0000\u001e\u001f\u0003\u0007\u0003\u0000\u001f\u0006"+
		"\u0001\u0000\u0000\u0000 \"\u0007\u0001\u0000\u0000! \u0001\u0000\u0000"+
		"\u0000\"#\u0001\u0000\u0000\u0000#!\u0001\u0000\u0000\u0000#$\u0001\u0000"+
		"\u0000\u0000$\b\u0001\u0000\u0000\u0000%&\u0007\u0002\u0000\u0000&\'\u0003"+
		"\u0005\u0002\u0000\'\n\u0001\u0000\u0000\u0000(*\u0003\u0007\u0003\u0000"+
		")+\u0003\t\u0004\u0000*)\u0001\u0000\u0000\u0000*+\u0001\u0000\u0000\u0000"+
		"+5\u0001\u0000\u0000\u0000,.\u0003\u0007\u0003\u0000-,\u0001\u0000\u0000"+
		"\u0000-.\u0001\u0000\u0000\u0000./\u0001\u0000\u0000\u0000/0\u0005.\u0000"+
		"\u000002\u0003\u0007\u0003\u000013\u0003\t\u0004\u000021\u0001\u0000\u0000"+
		"\u000023\u0001\u0000\u0000\u000035\u0001\u0000\u0000\u00004(\u0001\u0000"+
		"\u0000\u00004-\u0001\u0000\u0000\u00005\f\u0001\u0000\u0000\u000067\u0005"+
		"+\u0000\u00007\u000e\u0001\u0000\u0000\u000089\u0005-\u0000\u00009\u0010"+
		"\u0001\u0000\u0000\u0000:;\u0005*\u0000\u0000;\u0012\u0001\u0000\u0000"+
		"\u0000<=\u0005/\u0000\u0000=\u0014\u0001\u0000\u0000\u0000>@\u0007\u0003"+
		"\u0000\u0000?>\u0001\u0000\u0000\u0000@A\u0001\u0000\u0000\u0000A?\u0001"+
		"\u0000\u0000\u0000AB\u0001\u0000\u0000\u0000BC\u0001\u0000\u0000\u0000"+
		"CD\u0006\n\u0000\u0000D\u0016\u0001\u0000\u0000\u0000\b\u0000\u001c#*"+
		"-24A\u0001\u0006\u0000\u0000";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}