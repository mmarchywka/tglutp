#ifndef DCDLIB_H__
#define DCDLIB_H__

#include <stdio.h>
#include <stdlib.h>
// downloaded from http://people.scs.fsu.edu/~burkardt/cpp_src/dcdflib/dcdflib.html
// ca. March 2008
/*List of Routines: 

ALGDIV computes ln ( Gamma ( B ) / Gamma ( A + B ) ) when 8 <= B. 
ALNREL evaluates the function ln ( 1 + A ). 
APSER computes the incomplete beta ratio I(SUB(1-X))(B,A). 
BCORR evaluates DEL(A0) + DEL(B0) - DEL(A0 + B0). 
BETA evaluates the beta function. 
BETA_ASYM computes an asymptotic expansion for IX(A,B), for large A and B. 
BETA_FRAC evaluates a continued fraction expansion for IX(A,B). 
BETA_GRAT evaluates an asymptotic expansion for IX(A,B). 
BETA_INC evaluates the incomplete beta function IX(A,B). 
BETA_INC_VALUES returns some values of the incomplete Beta function. 
BETA_LOG evaluates the logarithm of the beta function. 
BETA_PSER uses a power series expansion to evaluate IX(A,B)(X). 
BETA_RCOMP evaluates X**A * Y**B / Beta(A,B). 
BETA_RCOMP1 evaluates exp(MU) * X**A * Y**B / Beta(A,B). 
BETA_UP evaluates IX(A,B) - IX(A+N,B) where N is a positive integer. 
BINOMIAL_CDF_VALUES returns some values of the binomial CDF. 
CDFBET evaluates the CDF of the Beta Distribution. 
CDFBIN evaluates the CDF of the Binomial distribution. 
CDFCHI evaluates the CDF of the chi square distribution. 
CDFCHN evaluates the CDF of the Noncentral Chi-Square. 
CDFF evaluates the CDF of the F distribution. 
CDFFNC evaluates the CDF of the Noncentral F distribution. 
CDFGAM evaluates the CDF of the Gamma Distribution. 
CDFNBN evaluates the CDF of the Negative Binomial distribution 
CDFNOR evaluates the CDF of the Normal distribution. 
CDFPOI evaluates the CDF of the Poisson distribution. 
CDFT evaluates the CDF of the T distribution. 
CHI_NONCENTRAL_CDF_VALUES returns values of the noncentral chi CDF. 
CHI_SQUARE_CDF_VALUES returns some values of the Chi-Square CDF. 
CUMBET evaluates the cumulative incomplete beta distribution. 
CUMBIN evaluates the cumulative binomial distribution. 
CUMCHI evaluates the cumulative chi-square distribution. 
CUMCHN evaluates the cumulative noncentral chi-square distribution. 
CUMF evaluates the cumulative F distribution. 
CUMFNC evaluates the cumulative noncentral F distribution. 
CUMGAM evaluates the cumulative incomplete gamma distribution. 
CUMNBN evaluates the cumulative negative binomial distribution. 
CUMNOR computes the cumulative normal distribution. 
CUMPOI evaluates the cumulative Poisson distribution. 
CUMT evaluates the cumulative T distribution. 
DBETRM computes the Sterling remainder for the complete beta function. 
DEXPM1 evaluates the function EXP(X) - 1. 
DINVNR computes the inverse of the normal distribution. 
DINVR bounds the zero of the function and invokes DZROR. 
DLANOR evaluates the logarithm of the asymptotic Normal CDF. 
DPMPAR provides machine constants for double precision arithmetic. 
DSTINV seeks a value X such that F(X) = Y. 
DSTREM computes the Sterling remainder ln ( Gamma ( Z ) ) - Sterling ( Z ). 
DSTXR sets quantities needed by the zero finder. 
DT1 computes an approximate inverse of the cumulative T distribution. 
DZROR seeks the zero of a function using reverse communication. 
E0000 is a reverse-communication zero bounder. 
E00001 is a reverse-communication zero finder. 
ERF_VALUES returns some values of the ERF or "error" function. 
ERROR_F evaluates the error function ERF. 
ERROR_FC evaluates the complementary error function ERFC. 
ESUM evaluates exp ( MU + X ). 
EVAL_POL evaluates a polynomial at X. 
EXPARG returns the largest or smallest legal argument for EXP. 
F_CDF_VALUES returns some values of the F CDF test function. 
F_NONCENTRAL_CDF_VALUES returns some values of the F CDF test function. 
FIFDINT truncates a double number to an integer. 
FIFDMAX1 returns the maximum of two numbers a and b 
FIFDMIN1 returns the minimum of two numbers. 
FIFDSIGN transfers the sign of the variable "sign" to the variable "mag" 
FIFIDINT truncates a double number to a long integer 
FIFMOD returns the modulo of a and b 
FPSER evaluates IX(A,B)(X) for very small B. 
FTNSTOP prints a message to standard error and then exits. 
GAM1 computes 1 / GAMMA(A+1) - 1 for -0.5D+00 <= A <= 1.5 
GAMMA_INC evaluates the incomplete gamma ratio functions P(A,X) and Q(A,X). 
GAMMA_INC_INV computes the inverse incomplete gamma ratio function. 
GAMMA_INC_VALUES returns some values of the incomplete Gamma function. 
GAMMA_LN1 evaluates ln ( Gamma ( 1 + A ) ), for -0.2 <= A <= 1.25. 
GAMMA_LOG evaluates ln ( Gamma ( A ) ) for positive A. 
GAMMA_RAT1 evaluates the incomplete gamma ratio functions P(A,X) and Q(A,X). 
GAMMA_VALUES returns some values of the Gamma function. 
GAMMA_X evaluates the gamma function. 
GSUMLN evaluates the function ln(Gamma(A + B)). 
IPMPAR returns integer machine constants. 
NEGATIVE_BINOMIAL_CDF_VALUES returns values of the negative binomial CDF. 
NORMAL_CDF_VALUES returns some values of the Normal CDF. 
POISSON_CDF_VALUES returns some values of the Poisson CDF. 
PSI evaluates the psi or digamma function, d/dx ln(gamma(x)). 
PSI_VALUES returns some values of the Psi or Digamma function. 
RCOMP evaluates exp(-X) * X**A / Gamma(A). 
REXP evaluates the function EXP(X) - 1. 
RLOG computes X - 1 - LN(X). 
RLOG1 evaluates the function X - ln ( 1 + X ). 
STUDENT_CDF_VALUES returns some values of the Student CDF. 
STVALN provides starting values for the inverse of the normal distribution. 
TIMESTAMP prints the current YMDHMS date as a time stamp. 

You can go up one level to the C++ source codes. 
*/
namespace dcdflib
{
double algdiv ( double *a, double *b );
double alnrel ( double *a );
double apser ( double *a, double *b, double *x, double *eps );
double bcorr ( double *a0, double *b0 );
double beta ( double a, double b );
double beta_asym ( double *a, double *b, double *lambda, double *eps );
double beta_frac ( double *a, double *b, double *x, double *y, double *lambda,
  double *eps );
void beta_grat ( double *a, double *b, double *x, double *y, double *w,
  double *eps,int *ierr );
void beta_inc ( double *a, double *b, double *x, double *y, double *w,
  double *w1, int *ierr );
void beta_inc_values ( int *n_data, double *a, double *b, double *x, double *fx );
double beta_log ( double *a0, double *b0 );
double beta_pser ( double *a, double *b, double *x, double *eps );
double beta_rcomp ( double *a, double *b, double *x, double *y );
double beta_rcomp1 ( int *mu, double *a, double *b, double *x, double *y );
double beta_up ( double *a, double *b, double *x, double *y, int *n, double *eps );
void binomial_cdf_values ( int *n_data, int *a, double *b, int *x, double *fx );
void cdfbet ( int *which, double *p, double *q, double *x, double *y,
  double *a, double *b, int *status, double *bound );
void cdfbin ( int *which, double *p, double *q, double *s, double *xn,
  double *pr, double *ompr, int *status, double *bound );
void cdfchi ( int *which, double *p, double *q, double *x, double *df,
  int *status, double *bound );
void cdfchn ( int *which, double *p, double *q, double *x, double *df,
  double *pnonc, int *status, double *bound );
void cdff ( int *which, double *p, double *q, double *f, double *dfn,
  double *dfd, int *status, double *bound );
void cdffnc ( int *which, double *p, double *q, double *f, double *dfn,
  double *dfd, double *phonc, int *status, double *bound );
void cdfgam ( int *which, double *p, double *q, double *x, double *shape,
  double *scale, int *status, double *bound );
void cdfnbn ( int *which, double *p, double *q, double *s, double *xn,
  double *pr, double *ompr, int *status, double *bound );
void cdfnor ( int *which, double *p, double *q, double *x, double *mean,
  double *sd, int *status, double *bound );
void cdfpoi ( int *which, double *p, double *q, double *s, double *xlam,
  int *status, double *bound );
void cdft ( int *which, double *p, double *q, double *t, double *df,
  int *status, double *bound );
void chi_noncentral_cdf_values ( int *n_data, double *x, double *lambda, 
  int *df, double *cdf );
void chi_square_cdf_values ( int *n_data, int *a, double *x, double *fx );
void cumbet ( double *x, double *y, double *a, double *b, double *cum,
  double *ccum );
void cumbin ( double *s, double *xn, double *pr, double *ompr,
  double *cum, double *ccum );
void cumchi ( double *x, double *df, double *cum, double *ccum );
void cumchn ( double *x, double *df, double *pnonc, double *cum,
  double *ccum );
void cumf ( double *f, double *dfn, double *dfd, double *cum, double *ccum );
void cumfnc ( double *f, double *dfn, double *dfd, double *pnonc,
  double *cum, double *ccum );
void cumgam ( double *x, double *a, double *cum, double *ccum );
void cumnbn ( double *s, double *xn, double *pr, double *ompr,
  double *cum, double *ccum );
void cumnor ( double *arg, double *result, double *ccum );
void cumpoi ( double *s, double *xlam, double *cum, double *ccum );
void cumt ( double *t, double *df, double *cum, double *ccum );
double dbetrm ( double *a, double *b );
double dexpm1 ( double *x );
double dinvnr ( double *p, double *q );
void dinvr ( int *status, double *x, double *fx,
  unsigned long *qleft, unsigned long *qhi );
double dlanor ( double *x );
double dpmpar ( int *i );
void dstinv ( double *zsmall, double *zbig, double *zabsst,
  double *zrelst, double *zstpmu, double *zabsto, double *zrelto );
double dstrem ( double *z );
void dstzr ( double *zxlo, double *zxhi, double *zabstl, double *zreltl );
double dt1 ( double *p, double *q, double *df );
void dzror ( int *status, double *x, double *fx, double *xlo,
  double *xhi, unsigned long *qleft, unsigned long *qhi );
//static 
void E0000 ( int IENTRY, int *status, double *x, double *fx,
  unsigned long *qleft, unsigned long *qhi, double *zabsst,
  double *zabsto, double *zbig, double *zrelst,
  double *zrelto, double *zsmall, double *zstpmu );
//static 
void E0001 ( int IENTRY, int *status, double *x, double *fx,
  double *xlo, double *xhi, unsigned long *qleft,
  unsigned long *qhi, double *zabstl, double *zreltl,
  double *zxhi, double *zxlo );
void erf_values ( int *n_data, double *x, double *fx );
double error_f ( double *x );
double error_fc ( int *ind, double *x );
double esum ( int *mu, double *x );
double eval_pol ( double a[], int *n, double *x );
double exparg ( int *l );
void f_cdf_values ( int *n_data, int *a, int *b, double *x, double *fx );
void f_noncentral_cdf_values ( int *n_data, int *a, int *b, double *lambda, 
  double *x, double *fx );
double fifdint ( double a );
double fifdmax1 ( double a, double b );
double fifdmin1 ( double a, double b );
double fifdsign ( double mag, double sign );
long fifidint ( double a );
long fifmod ( long a, long b );
double fpser ( double *a, double *b, double *x, double *eps );
void ftnstop ( char *msg );
double gam1 ( double *a );
void gamma_inc ( double *a, double *x, double *ans, double *qans, int *ind );
void gamma_inc_inv ( double *a, double *x, double *x0, double *p, double *q,
  int *ierr );
void gamma_inc_values ( int *n_data, double *a, double *x, double *fx );
double gamma_ln1 ( double *a );
double gamma_log ( double *a );
void gamma_rat1 ( double *a, double *x, double *r, double *p, double *q,
  double *eps );
void gamma_values ( int *n_data, double *x, double *fx );
double gamma_x ( double *a );
double gsumln ( double *a, double *b );
int ipmpar ( int *i );
void negative_binomial_cdf_values ( int *n_data, int *f, int *s, double *p, 
  double *cdf );
void normal_cdf_values ( int *n_data, double *x, double *fx );
void poisson_cdf_values ( int *n_data, double *a, int *x, double *fx );
double psi ( double *xx );
void psi_values ( int *n_data, double *x, double *fx );
double rcomp ( double *a, double *x );
double rexp ( double *x );
double rlog ( double *x );
double rlog1 ( double *x );
void student_cdf_values ( int *n_data, int *a, double *x, double *fx );
double stvaln ( double *p );
void timestamp ( void );

};
#endif

