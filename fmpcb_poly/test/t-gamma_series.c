/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2012, 2013 Fredrik Johansson

******************************************************************************/

#include "fmpcb_poly.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("gamma_series....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 1000; iter++)
    {
        long m, n1, n2, rbits1, rbits2, rbits3;
        fmpcb_poly_t a, b, c, d;

        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);
        rbits3 = 2 + n_randint(state, 200);

        m = 1 + n_randint(state, 25);
        n1 = 1 + n_randint(state, 25);
        n2 = 1 + n_randint(state, 25);

        fmpcb_poly_init(a);
        fmpcb_poly_init(b);
        fmpcb_poly_init(c);
        fmpcb_poly_init(d);

        fmpcb_poly_randtest(a, state, m, rbits1, 3);

        fmpcb_poly_gamma_series(b, a, n1, rbits2);
        fmpcb_poly_gamma_series(c, a, n2, rbits3);

        fmpcb_poly_set(d, b);
        fmpcb_poly_truncate(d, FLINT_MIN(n1, n2));
        fmpcb_poly_truncate(c, FLINT_MIN(n1, n2));

        if (!fmpcb_poly_overlaps(c, d))
        {
            printf("FAIL\n\n");
            printf("n1 = %ld, n2 = %ld, bits2 = %ld, bits3 = %ld\n", n1, n2, rbits2, rbits3);

            printf("a = "); fmpcb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); fmpcb_poly_printd(b, 15); printf("\n\n");
            printf("c = "); fmpcb_poly_printd(c, 15); printf("\n\n");

            abort();
        }

        /* check gamma(a) * a = gamma(a+1) */
        fmpcb_poly_mullow(c, b, a, n1, rbits2);

        fmpcb_poly_set(d, a);
        fmpcb_add_ui(d->coeffs, d->coeffs, 1, rbits2);
        fmpcb_poly_gamma_series(d, d, n1, rbits2);

        if (!fmpcb_poly_overlaps(c, d))
        {
            printf("FAIL (functional equation, n1 = %ld)\n\n", n1);

            printf("a = "); fmpcb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); fmpcb_poly_printd(b, 15); printf("\n\n");
            printf("c = "); fmpcb_poly_printd(c, 15); printf("\n\n");
            printf("d = "); fmpcb_poly_printd(d, 15); printf("\n\n");

            abort();
        }

        fmpcb_poly_gamma_series(a, a, n1, rbits2);
        if (!fmpcb_poly_overlaps(a, b))
        {
            printf("FAIL (aliasing)\n\n");
            abort();
        }

        fmpcb_poly_clear(a);
        fmpcb_poly_clear(b);
        fmpcb_poly_clear(c);
        fmpcb_poly_clear(d);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}

