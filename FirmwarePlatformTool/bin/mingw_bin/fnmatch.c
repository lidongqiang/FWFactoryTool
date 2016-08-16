#include <fnmatch.h>
#undef	EOS
#define	EOS	'\0'
#define	RANGE_MATCH	1
#define	RANGE_NOMATCH	0
#define	RANGE_ERROR	(-1)
#define ISSET(a,b) (a&b)
static int rangematch(const char *pattern, char test, int flags, char **newp)
{
	int negate, ok;
	char c, c2;

	/*
	 * A bracket expression starting with an unquoted circumflex
	 * character produces unspecified results (IEEE 1003.2-1992,
	 * 3.13.2).  This implementation treats it like '!', for
	 * consistency with the regular expression syntax.
	 * J.T. Conklin (conklin@ngai.kaleida.com)
	 */
	if ((negate = (*pattern == '!' || *pattern == '^')))
		++pattern;

	if (ISSET(flags, FNM_CASEFOLD))
		test = tolower((unsigned char)test);

	/*
	 * A right bracket shall lose its special meaning and represent
	 * itself in a bracket expression if it occurs first in the list.
	 * -- POSIX.2 2.8.3.2
	 */
	ok = 0;
	c = *pattern++;
	do {
		if (c == '\\' && !ISSET(flags, FNM_NOESCAPE))
			c = *pattern++;
		if (c == EOS)
			return (int)(RANGE_ERROR);
		if (c == '/' && ISSET(flags, FNM_PATHNAME))
			return (int)(RANGE_NOMATCH);
		if (ISSET(flags, FNM_CASEFOLD))
			c = tolower((unsigned char)c);
		if (*pattern == '-'
		    && (c2 = *(pattern+1)) != EOS && c2 != ']') {
			pattern += 2;
			if (c2 == '\\' && !ISSET(flags, FNM_NOESCAPE))
				c2 = *pattern++;
			if (c2 == EOS)
				return (int)(RANGE_ERROR);
			if (ISSET(flags, FNM_CASEFOLD))
				c2 = tolower((unsigned char)c2);
			if (c <= test && test <= c2)
				ok = 1;
		} else if (c == test)
			ok = 1;
	} while ((c = *pattern++) != ']');

	*newp = (char *)pattern;
	return (int)(ok == negate ? RANGE_NOMATCH : RANGE_MATCH);
}
int fnmatch(pattern, string, flags)
const char *pattern, *string;
int flags;
{
	char *stringstart;
	char *newp;
	char c, test;

	for (stringstart = string;;)
		switch (c = *pattern++) {
		case EOS:
			if (ISSET(flags, FNM_LEADING_DIR) && *string == '/')
				return (0);
			return (*string == EOS ? 0 : FNM_NOMATCH);
		case '?':
			if (*string == EOS)
				return (FNM_NOMATCH);
			if (*string == '/' && ISSET(flags, FNM_PATHNAME))
				return (FNM_NOMATCH);
			if (*string == '.' && ISSET(flags, FNM_PERIOD) &&
			    (string == stringstart ||
			    (ISSET(flags, FNM_PATHNAME) && *(string - 1) == '/')))
				return (FNM_NOMATCH);
			++string;
			break;
		case '*':
			c = *pattern;
			/* Collapse multiple stars. */
			while (c == '*')
				c = *++pattern;

			if (*string == '.' && ISSET(flags, FNM_PERIOD) &&
			    (string == stringstart ||
			    (ISSET(flags, FNM_PATHNAME) && *(string - 1) == '/')))
				return (FNM_NOMATCH);

			/* Optimize for pattern with * at end or before /. */
			if (c == EOS) {
				if (ISSET(flags, FNM_PATHNAME))
					return (ISSET(flags, FNM_LEADING_DIR) ||
					    strchr(string, '/') == NULL ?
					    0 : FNM_NOMATCH);
				else
					return (0);
			} else if (c == '/' && ISSET(flags, FNM_PATHNAME)) {
				if ((string = strchr(string, '/')) == NULL)
					return (FNM_NOMATCH);
				break;
			}

			/* General case, use recursion. */
			while ((test = *string) != EOS) {
				if (!fnmatch(pattern, string, flags & ~FNM_PERIOD))
					return (0);
				if (test == '/' && ISSET(flags, FNM_PATHNAME))
					break;
				++string;
			}
			return (FNM_NOMATCH);
		case '[':
			if (*string == EOS)
				return (FNM_NOMATCH);
			if (*string == '/' && ISSET(flags, FNM_PATHNAME))
				return (FNM_NOMATCH);
			if (*string == '.' && ISSET(flags, FNM_PERIOD) &&
			    (string == stringstart ||
			    (ISSET(flags, FNM_PATHNAME) && *(string - 1) == '/')))
				return (FNM_NOMATCH);

			switch (rangematch(pattern, *string, flags, &newp)) {
			case RANGE_ERROR:
				/* not a good range, treat as normal text */
				goto normal;
			case RANGE_MATCH:
				pattern = newp;
				break;
			case RANGE_NOMATCH:
				return (FNM_NOMATCH);
			}
			++string;
			break;
		case '\\':
			if (!ISSET(flags, FNM_NOESCAPE)) {
				if ((c = *pattern++) == EOS) {
					c = '\\';
					--pattern;
				}
			}
			/* FALLTHROUGH */
		default:
		normal:
			if (c != *string && !(ISSET(flags, FNM_CASEFOLD) &&
				 (tolower((unsigned char)c) ==
				 tolower((unsigned char)*string))))
				return (FNM_NOMATCH);
			++string;
			break;
		}
	/* NOTREACHED */
}