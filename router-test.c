/* ------------------------------------------------------ *
 * router-test.c
 * =============
 * 
 * Test that simple route resolution works.
 * 
 * You can pass in a custom receiver function if the datatype
 * is not something simple like a list of strings.
 *
 * ------------------------------------------------------ */
//Compile me with: 
//gcc -ldl -lpthread -o router vendor/single.o vendor/sqlite3.o router.c && ./router
#include <zwalker.h>
#include "router.h"

#define TESTCOUNT 14

#define HELP \
	"help menu: \n" \
	"-h, --help           Show help.\n" \
	"-u, --uri <arg>      Supply a uri for testing.\n" \
	"-t, --case <arg>     Supply a test route to match against.\n" \
	"                     (use multiple -t flags to specify multiple tests).\n"

//Simple list of strings
const char *routes_basic[] = {
	"/",
	"//",
	"/route",
	"/route/",
	"/route",
	"/route/:id",
	"/route/:id=string",
	"/route/:id=number",
	"/:id",
	"/route/*",
	"/route/*/jackpot",
	"/{route,julius}",
	"/{route,julius}/:id",
#if 0
//Do I want to handle '?'
, "/route/?"
, "/route/?accharat"
, "/route/??"
, "/route/???"
//Do I want to handle regular expressions?
//, "/route/[^a-z]"
#endif
	NULL
};



//Simple list of slightly complex datatypes
struct useless {
	const char *name;
	int useless_int;
	void * useless_void;
} routes_advanced [] = {
	{ "/" },                   /* 1 */
	{ "//" },                  /* 2 */
	{ "/route" },              /* 3 */
	{ "/route/" },             /* 4 */
	{ "/route" },              /* 5 */
	{ "/route/:id" },          /* 6 */
	{ "/route/:id=string" },   /* 7 */
	{ "/route/:id=number" },   /* 8 */
	{ "/:id" },                /* 9 */
	{ "/route/*" },            /* 10 */
	{ "/route/*/jackpot" },    /* 11 */
	{ "/{route,julius}" },     /* 12 */
	{ "/{route,julius}/:id" }, /* 13 */
#if 0
//Do I want to handle '?'
, "/route/?"
, "/route/?accharat"
, "/route/??"
, "/route/???"
//Do I want to handle regular expressions?
//, "/route/[^a-z]"
#endif
	{ NULL }
};



const char * get_ustring ( void *u ) {
	fprintf( stderr, "%p\n", u );
	struct useless * uu = u;
	return uu->name;
}



#if 0
//These are possible request lines
const char *requests[] = {
  "/"
, "/2"
, "/route"
, "/route/3"
, "/route/bashful"
, "/route/bashful/jackpot"
, "/route/333/jackpot"
, "/julius/3"
, "/unresolvable"
, "/joseph/route/337"
, "/route/337a"
, "/route/3"
, NULL
};
#endif

struct request {
	const char *uri; 
	char cmp[ TESTCOUNT ];
} requests[] = {
	//URI is on the left, the test cases are on the right
  { "/",                       "YYNNNNNNNNNNN" }
, { "/2",                      "NNNNNNNNYNNNN" }
, { "/47",                     "NNNNNNNNYNNNN" }
, { "/route",                  "NNYYYNNNYNNYN" }
, { "/route/3",                "NNNNNYNYNYNNY" }
, { "/route/bashful",          "NNNNNYYNNYNNY" }
, { "/route/bashful/jackpot",  "NNNNNNNNNNYNN" }
, { "/route/333/jackpot",      "NNNNNNNNNNYNN" }
, { "/julius/3",               "NNNNNNNNNNNNY" }
, { "/unresolvable",           "NNNNNNNNYNNNN" }
, { "/joseph/route/337",       "NNNNNNNNNNNNN" }
, { "/route/337a",             "NNNNNYYNNYNNY" }
, { NULL }
};


int main (int argc, char *argv[]) {
	//Add a command line argument that can go through the test cases
	//or more sensibly one or more different cases from the command line
	if ( argc > 1 ) {
		char *uri = NULL, *test[32] = { NULL };
		char **testcase = test;
		int standard = 0;

		while ( *argv ) {
			if ( !strcmp( *argv, "-u" ) ||!strcmp( *argv, "--uri" ) )
				uri = *( ++argv );	
			else if ( !strcmp( *argv, "-t" ) ||!strcmp( *argv, "--case" ) )
				*testcase = *( ++argv );
			else if ( !strcmp( *argv, "-s" ) ||!strcmp( *argv, "--standard" ) )
				standard = 1;
			else if ( !strcmp( *argv, "-h" ) ||!strcmp( *argv, "--help" ) ) {
				fprintf( stderr, "%s\n", HELP ); 
				return 1;
			}
			argv++;
		}

		if ( !uri ) {
			fprintf( stderr, "No test uri given." );
			return 1;
		}

		//run standard if testcase was not specified
		if ( standard || ( standard = !testcase ) ) {
			for ( const char **r = routes_basic; *r; r++ ) {
				fprintf( stderr, "%s vs %s = ", uri, *r );
				fprintf( stderr, "%c\n", route_resolve( uri, *r ) ? 'Y' : 'N' );
			}
		}
		else {
			if ( !*testcase ) {
				fprintf( stderr, "No test case given." );
				return 1;
			}
			
			if ( !route_resolve( uri, *testcase ) ) { 
				fprintf( stderr, "Route %s does not match case %s\n", uri, *testcase );
				return 1;
			}

			fprintf( stderr, "Route %s matches case %s\n", uri, *testcase );
		}

		return 0;
	}

	//Start complex datatype tests
	fprintf( stderr, "\nROUTING MATCHES VIA COMPLEX TYPES\n" );
	fprintf( stderr, "=================================\n" );

	//The long (and slow) way to do it
	for ( struct request *r = requests; r->uri; r++ ) {
		char *cmp, *fmp, cmpbuf[ TESTCOUNT ] = { 0 };
		fmp = cmp = cmpbuf;
		//fprintf( stderr, "%s\n", r->uri );
		//struct useless *resv = NULL;
		int routecount = sizeof( routes_advanced ) / sizeof( struct useless );
		fprintf( stderr,  "Checking %d routes against this URI: %s\n", routecount, r->uri );

		for ( struct useless *uu = routes_advanced; uu->name ; uu++ ) {
			*cmp = (route_resolve( r->uri, uu->name )) ? 'Y' : 'N', cmp++;
		}

		fprintf( stderr, "%s, %s: ", fmp, r->cmp );
 
		if ( memcmp( fmp, r->cmp, TESTCOUNT ) == 0 )
			fprintf( stderr, "\n" );
		else {
			fprintf( stderr, "TEST for %s FAILED!\n", r->uri );
			fprintf( stderr, "'%s' should have resolved to:\n", r->uri );
#if 1
			int c = 0;
			fprintf( stderr, "%30s  =>  %c    %c\n", "route", 'E', 'R' );
			fprintf( stderr, "%43s\n", "==================" );
			for ( struct useless *uu = routes_advanced; uu->name ; uu++, c++ ) {
				fprintf( stderr, "%30s  =>  %c %c= %c\n", 
					uu->name, r->cmp[c], (r->cmp[c] == fmp[c]) ? '=' : '!', fmp[c] ); 
			}
#endif
			return 1;
		}
	}
	return 0;
}
