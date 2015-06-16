
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void indexToTuple(int n, int size, int index, int* tuple);
bool has_standard_example(int n, char* A, int k );
bool has_dimension_k(char* s, int n, char* A, int k );

char* A_g = 0;

int main(int argc, char** argv)
{
	int k = 4;
	int n = 9;


	for ( int i = 0; i < argc; i++ )
	{
		if ( strcmp(argv[i],"-k") == 0 )
		{
			k = atoi(argv[i+1]);
		}
		if ( strcmp(argv[i],"-n") == 0 )
		{
			n = atoi(argv[i+1]);
		}
	}

	A_g = (char*)malloc(n * n);


	size_t buflen = 500;
	char* buffer = (char*)malloc(buflen);

	while ( getline(&buffer, &buflen, stdin) > 0 )
	{
		if ( strlen(buffer) <= 3 )
		{
			break;
		}

		for ( int i = 0; i < n; i++ )
		{
			for ( int j = 0; j < n; j++ )
			{
				A_g[i*n+j] = buffer[ n * i + j] - '0';
			}
		}
		buffer[n*n] = 0;

		if ( !has_standard_example(n, A_g, k) )
		{
			if ( !has_dimension_k(buffer, n, A_g, k-1) )
			{
				printf("%s has dimension > %d but no %d-standard example!\n", buffer, k-1, k);
			}
		}
		else
		{
			printf("%s contains %d-standard example!\n", buffer, k);
		}

		fflush(stdout);
	}


	free(buffer);

	return 0;
}

bool has_standard_example(int n, char* A, int k )
{
	if ( n != 2 * k + 1 )
	{
		fprintf(stderr, "[has_standard_example] is only implemented when |P| = 2k+1!\n");
		exit(1);
	}


	for ( int r = 0; r < n; r++ )
	{
		int num_with_in_deg = 0;
		int num_with_out_deg = 0;

		bool works = true;

		for ( int i = 0; i < n; i++ )
		{
			if ( i == r )
			{
				continue;
			}

			int din = 0;
			int dout = 0;

			for ( int j = 0; j < n; j++ )
			{
				if ( j == r )
				{
					continue;
				}

				din += A[j*n+i];
				dout += A[i*n+j];
			}

			if ( din == k - 1 && dout == 0 )
			{
				num_with_in_deg++;
				if ( num_with_in_deg > k )
				{
					works = false;
				}
			}
			else if ( din == 0 && dout == k - 1 )
			{
				num_with_out_deg++;
				if ( num_with_out_deg > k )
				{
					works = false;
				}
			}
			else
			{
				works = false;
			}
		}

		if ( works )
		{
			return true;
		}
	}

	return false;
}

int* has_dimension_k_recurse(int n, char* A, int k, int* linear_extensions, int i)
{
	int* next_linear_extensions = linear_extensions;

	if ( i >= n )
	{
		// Are we done?
		return linear_extensions;
	}
	else if ( i > 0 )
	{
		next_linear_extensions = &(linear_extensions[k * n]);

		// ok, now try all options.. but let's compute our bounds!
		int num_tuples = 1;
		for ( int j = 0; j < k; j++ )
		{
			num_tuples *= (i+1);
		}

		int* insertion = (int*)malloc(k * sizeof(int));

		// Note: If any immediate improvement is needed, then pre-determine the range of the tuple entries
		// and reduce the number of tuples to check here.
		for ( int t = 0; t < num_tuples; t++ )
		{
			indexToTuple(i+1, k, t, insertion);

			bool tuple_works = true;

			for ( int j = 0; tuple_works &&  j < k; j++ )
			{
				for ( int p = 0; tuple_works && p < insertion[j]; p++ )
				{
					if ( A[ i * n + linear_extensions[j * n + p] ] == 1 )
					{
						// i -> j is reversed here!

						tuple_works = false;
					}
				}
				for ( int p = insertion[j]; tuple_works && p < i; p++ )
				{
					if ( A[ linear_extensions[j * n + p] * n + i] == 1 )
					{
						// j -> i is reversed here!
						tuple_works = false;
					}
				}
			}

			for ( int j = 0; j < i; j++ )
			{
				if ( A[ n * i + j ] == 0 && A[ n * j + i ] == 0  )
				{
					// i and j are incomparable!
					bool has_i_to_j = false;
					bool has_j_to_i = false;

					for ( int kk = 0; kk < k; kk++ )
					{
						for ( int jj = 0; jj < i; jj++ )
						{
							if ( linear_extensions[kk * n + jj] == j && jj < insertion[kk] )
							{
								has_j_to_i = true;
							}
							if ( linear_extensions[kk * n + jj] == j && jj >= insertion[kk] )
							{
								has_i_to_j = true;
							}
						}
					}

					if ( !has_i_to_j || !has_j_to_i )
					{
						// we do not show why this is incomparable!
						tuple_works = false;
					}
				}
			}

			if ( tuple_works )
			{
				// now copy the data!
				for ( int j = 0; j < k; j++ )
				{
					for ( int p = 0; p <= i; p++ )
					{
						if ( p < insertion[j] )
						{
							next_linear_extensions[ j * n + p ] = linear_extensions[ j * n + p];	
						}
						else if ( p == insertion[j] )
						{
							next_linear_extensions[ j * n + p ] = i;
						}
						else if ( p > insertion[j] )
						{
							next_linear_extensions[ j * n + p ] = linear_extensions[ j * n + p - 1 ];	
						}
					}	
				}

				int* result = has_dimension_k_recurse(n, A, k, next_linear_extensions, i + 1 );

				if ( result != 0 )
				{
					return result;
				}
			}
		}

		free(insertion);
	}
	else
	{
		// load with vertex zero
		for ( int i = 0; i < k; i++ )
		{
			linear_extensions[i * n] = 0;
		}

		return has_dimension_k_recurse(n, A, k, linear_extensions, 1);
	}

	return 0;
}

int* linear_extensions_g = 0;

/** 
 * Determine if the poset with comparability matrix A
 * has a k-dimensional realizer. If s is not null, then
 * the realizer is printed along with the string description.
 */
bool has_dimension_k(char* s, int n, char* A, int k )
{
	if ( linear_extensions_g == 0 )
	{
		linear_extensions_g = (int*)malloc( k * n * n * sizeof(int) );
		bzero(linear_extensions_g, k * n * n  * sizeof(int) );
	}

	int* result = has_dimension_k_recurse(n, A, k, linear_extensions_g, 0);

	if ( result != 0 )
	{
		if ( s != 0 )
		{
			printf("%s has dimension at most %d:\n", s, k);
			for (int i = 0; i < k; i++ )
			{
				for ( int j = 0; j < n; j++ )
				{
					printf("%2d", result[n * i + j]);
					if ( j < n - 1 )
					{
						printf(" < ");
					}
				}
				printf("\n");
			}
		}

		return true;
	}

	return false;
}





/**
 * indexToTuple
 *
 * Find the index-th size-tuple where the entries are in 0...n-1.
 * Place the result into "tuple".
 */
void indexToTuple( int n, int size, int index, int* tuple )
{
	if ( size == 0 )
	{
		return;
	}
	else if ( size == 1 )
	{
		tuple[0] = index;
	}
	else
	{
		int npow = pow(n, size - 1);
		tuple[size - 1] = (int) floor(index / npow);
		int base = npow * tuple[size - 1];
		int offset = index - base;
		indexToTuple(n, size - 1, offset, tuple);
	}
}


