#ifndef BISON_Y_TAB_H
# define BISON_Y_TAB_H

#ifndef YYSTYPE
typedef union {
int num;
char str[256];
int parvalue;
} yystype;
# define YYSTYPE yystype
#endif
# define	T_B_INPUT	257
# define	T_B_OUTPUT	258
# define	T_B_PINPUT	259
# define	T_B_POUTPUT	260
# define	T_B_NOT	261
# define	T_B_AND	262
# define	T_B_OR	263
# define	T_B_NAND	264
# define	T_B_NOR	265
# define	T_B_BUF	266
# define	T_B_BUS	267
# define	T_B_XOR	268
# define	T_B_XNOR	269
# define	T_B_MUX	270
# define	T_B_TRIH	271
# define	T_B_TRIL	272
# define	T_B_DFF	273
# define	T_B_SDFF	274
# define	T_B_CNF	275
# define	T_B_DC	276
# define	EOLINE	277
# define	LPAR	278
# define	RPAR	279
# define	LCURLY	280
# define	RCURLY	281
# define	COMMA	282
# define	EQUAL	283
# define	MINUS	284
# define	NAME	285
# define	NUMBER	286


extern YYSTYPE yylval;

#endif /* not BISON_Y_TAB_H */
