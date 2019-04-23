Poset Dimension Computation
===========================

This repo contains code to verify the following theorem.

**Theorem.** If _P_ is a poset of order 9, then
 the dimension of _P_ is at most 4, and the dimension
 of _P_ equals 4 if and only if P contains the
 standard example of dimension 4.

This is a special case of a more general theorem, but
no general proof of this specific case is known without
the computation described here.

After compiling `podim.exe` with `make`, you can test the values
by running `test-all.sh`, which runs the following
three commands:

```
./podim.exe -n 5 -k 2 < posets5.txt > p5-out.txt
./podim.exe -n 7 -k 3 < posets7.txt > p7-out.txt
./podim.exe -n 9 -k 4 < posets9.txt > p9-out.txt
```

On current personal computing hardware (such as a typical
laptop) this should complete in less than an hour.

The input files `posets<n>.txt` were generated using
Sage Math and its combinatorics library to generated
digraphs that are acyclic and transitive. That is, they
contain the adjacency matrices (given as a single row
vector) of the comparability digraphs for each poset of
order _n_, where _n_ is 5, 7, or 9.

The output files `p<n>-out.txt` list the input digraphs
along with one of the following statements:

* `<G> has dimension > <k-1> but no <k>-standard example!`
    This describes a case where the poset given by _G_
    has no standard example of dimension _k_, but has dimension
    at least _k_.

* `<G> has dimension at most <k-1>:`
    This describes a case where the poset given by _G_
    has dimension at most _k_-1, and this is shown by listing
    the _k_ - 1 linear extensions that show this upper bound.

* `<G> contains <k>-standard example!`
    This describes a case where the poset given by _G_
    contains the _k_-dimensional standard example, and hence
    its dimension is not computed.

For the case of _n_ in { 5, 7 } and _k_ in { 2, 3 } there
are examples in all three cases. For _n_ = 9 and _k_ = 4,
there are no cases where the dimension is at least _k_ but
does not contain the standard example of dimension _k_, thus
verifying the theorem.

We use a very simple backtrack search to find linear extensions
of size _k_ - 1.