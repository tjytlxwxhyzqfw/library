#ifndef _TJYTLXWXHYZQFW_COMGEO_C
#define _TJYTLXWXHYZQFW_COMGEO_C

/* untested */

int crossover_point(const double k1, const double b1, 
	const double k2, const double b2, 
	double *res_x, double *res_y)
{
	if (k1 == k2)
		return -1;

	*res_x = (b1 - b2) / (k2 - k1);
	*res_y = k1 * (*res_x) + b1;

	return 0;
}

int perpendicular_bisector(const double xa, const double ya,
	const double xb, const double yb,
	double *res_k, double *res_b)
{
	double mx, my;

	mx = (xa + xb) / 2;
	my = (ya + yb) / 2;

	if (yb == ya) {
		*res_k = mx;
		return 1;
	}

	*res_k = (xa - xb) / (yb - ya);
	*res_b = my - (*res_k) * mx;

	return 0;
}

#endif
