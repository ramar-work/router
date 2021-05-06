router
======

Simple C-based router.  Could be useful for someone.


## Usage

1. Add router.c, router.h and the <a href="https://github.com/zaiah-dj/zwalker">zwalker 
   library </a> to your project.

2. In your code, use <code>route_resolve( $your_uri, $matching_string )</code>
	 to resolve a route.  $your_uri and $matching_string are both zero-terminated strings.

3. route_resolve will simply return 0 or 1 (an int) depending on whether or not it
	 successfully matched your input URI.

Nothing should need to be done to the code to make it compile.   If anything 
is needed, it's most likely a bug and you can either submit a pull request or
email me at ramar@collinsdesign.net.


## Matching Rules

| string | matches                     |
| ------ | -------                     |
| /      | Matches any root request    | 
| /route | Matches request containing the word route and nothing else   | 
| /route/:id | Matches request containing the word route and any thing else at the next section | 
| /route/:id=number | Matches request containing the word route and a NUMERIC stub at the next section | 
| /\*/route | Matches request containing anything (synonymous with :id, but returns no key name), but the word route at the end |
| /{mario,luigi} | Matches request containing either the words 'mario' or 'luigi', but nothing else. |

## Testing

Run `make debug && bin/router` to run the tests.


## Changes

Some changes down the line:

- Use no heap.  (Mostly because its unnecessary.  URIs (even today) use sensible
  limits for identifying resources, and the underlying structures could very well
  use fixed size buffers instead of pointers to allocated data.) 

- Make it unicode ready.
  
