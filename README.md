router
======

Simple router.  Could be useful for someone.


## Usage

1. Add router.c, router.h and the zwalker library to your project.

2. In your code, use <code>route_resolve( $your_uri, $matching_string )</code>
	 to resolve a route.

3. route_resolve will simply return 0 or 1 depending on whether or not it
	 could match your input URI.

Nothing should need to be done to the code to make it compile.   If anything 
is needed, it's most likely a bug and you can either submit a pull request or
email me at ramar@collinsdesign.net.


## Testing

Run `make debug && bin/router` to run the tests.


## Changes

Some changes down the line:

- Use no heap.  (Mostly because its unnecessary.  URIs (even today) use sensible
  limits for identifying resources, and the underlying structures could very well
  use fixed size buffers instead of pointers to allocated data.) 

- Make it unicode ready.
  
