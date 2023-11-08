#include <stdio.h>
#include <math.h>

int main(void)
{
	double x_r = 150;
	double ms[] =
	{
		148,147,148,149,148,146
	};

	int n = sizeof(ms) / sizeof(*ms);
	printf("n = %d\n", n);

	/* Mittelwert */
	double x_strich = 0;
	for(int i = 0; i < n; ++i)
	{
		x_strich += ms[i];
	}

	x_strich /= n;
	printf("x_strich = %f\n", x_strich);

	/* Standardabweichung */
	double sum = 0;
	for(int i = 0; i < n; ++i)
	{
		double d = ms[i] - x_strich;
		sum += d * d;
	}

	sum /= (n * (n - 1));
	double sigma_strich = sqrt(sum);

	printf("sigma_strich = %f\n", sigma_strich);


	double t = 1.11; /* 68,3% bei 6 messungen */

	printf("t = %f\n", t);

	double v = t *  sigma_strich;

	printf("v = %f\n", v);

	double F = x_strich - x_r;

	printf("F = %f\n", F);

	double delta_u = F + v;

	printf("delta_u = %f\n", delta_u);

	printf("| XR | M1 | M2 | M3 | M4 | M5 | M6 | MW | VI |\n");
	printf("|%.0f|%.0f|%.0f|%.0f|%.0f|%.0f|%.0f|%.0f|%.0f|\n",
		x_r,
		ms[0], ms[1], ms[2], ms[3], ms[4], ms[5],
		x_strich, delta_u);

	return 0;
}