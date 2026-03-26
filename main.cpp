#include <stdio.h>
#include <stdlib.h>
#include <math.h>
const float twopi = 6.2831853071f;
struct taylor{
	int number; /*номер*/
	float current; /*нынешний член*/
	float x; /*аргумент*/ 
};
//void(*update_func)(struct taylor *step);

void update_exp(struct taylor *step) {
	step->number++;
	step->current *= (step->x / (float)step->number);
}
void update_cos(struct taylor* step) {
	step->number++;
	int doubled = step->number * 2;
	step->current *= -(step->x * step->x) / (float)((doubled - 1) * doubled);
}
void update_sin(struct taylor* step) {
	step->number++;
	int doubled_plus = 2 * step->number + 1;
	step->current *= -(step->x * step->x) / (float)((doubled_plus - 1) * doubled_plus);
}
void update_ln(struct taylor* step) {
	step->number++;
	step->current *= -step->x * (float)(step->number - 1) / (float)step->number;
}
float direct_sum(struct taylor step, void(*nextstep)(struct taylor*), int n) {
	float sum = 0.0f;
	for (int i = 0; i <= n; i++) {
		sum += step.current;
		nextstep(&step);
	}
	return sum;
}
float reverse_sum(struct taylor step,void(*nextstep)(taylor *step),int n){
	float sum = 0.0f;
	float* arr =(float*)malloc((n + 1) * sizeof(float));
	for (int i = 0; i < n + 1; i++) {
		arr[i] = step.current;
		nextstep(&step);
	}
	for (int i = n; i > -1; i--) {
		sum += arr[i];
	}
	free(arr);
	return sum;
}
float cahon_sum(struct taylor step,void(*nextstep)(taylor *step),int n){
	float sum = 0.0f;
	float diff = 0.0f;
	float z, t;
	for (int i = 0; i < n + 1; i++) {
		z = step.current - diff;
		t = sum + z;
		diff = (t - sum) - z;
		sum = t;
		nextstep(&step);
	}
	return sum;
}

int main() {
	struct taylor step;
	int n,choiec;
	float arg, res,res_reverse,res_cahon;
	printf("Choose function\n1)exp^x\n2)sinx\n3)cosx\n4)ln(1+x)\n");
	scanf_s("%i", &choiec);
	printf("Enter x: ");
	scanf_s("%f", &arg);
	printf("Enter n: ");
	scanf_s("%d", &n);
	if (choiec == 2 || choiec == 3) {
		while (arg >= twopi) {
			arg -= twopi;
		}
		while (arg < 0) {
			arg += twopi;
		}
	}
	switch (choiec) {
		case 1:
			step.x = arg;
			step.current = 1.0f;
			step.number = 0;
			res = direct_sum(step, update_exp, n);
			res_reverse = reverse_sum(step, update_exp, n);
			res_cahon = cahon_sum(step, update_exp, n);
			printf("Result direct: %f Error:%f\n", res, fabsf(expf(arg) - res));
			printf("Result reverse: %f Error:%f\n", res_reverse, fabsf(expf(arg) - res_reverse));
			printf("Result cahon: %f Error:%f\n", res_cahon, fabsf(expf(arg) - res_cahon));
			printf("Math.h: %f\n", expf(arg));
			break;

		case 2:
			step.x = arg;
			step.current = arg;
			step.number = 0;
			res = direct_sum(step, update_sin, n);
			res_reverse = reverse_sum(step, update_sin, n);
			res_cahon = cahon_sum(step, update_sin, n);
			printf("Result direct: %f Error:%f\n", res, fabsf(sinf(arg) - res));
			printf("Result reverse: %f Error:%f\n", res_reverse, fabsf(sinf(arg) - res_reverse));
			printf("Result cahon: %f Error:%f\n", res_cahon, fabsf(sinf(arg) - res_cahon));
			printf("Math.h: %f\n", sinf(arg));
			break;

		case 3:
			step.x = arg;
			step.current = 1.0f;
			step.number = 0;
			res_reverse = reverse_sum(step, update_cos, n);
			res = direct_sum(step, update_cos, n);
			res_cahon = cahon_sum(step, update_cos, n);
			printf("Result direct: %f Error:%f\n", res, fabsf(cosf(arg) - res));
			printf("Result reverse: %f Error:%f\n", res_reverse, fabsf(cosf(arg) - res_reverse));
			printf("Result cahon: %f Error:%f\n", res_cahon, fabsf(cosf(arg) - res_cahon));
			printf("Math.h: %f\n", cosf(arg));
			break;

		case 4:
			step.x = arg;
			step.current = arg;
			step.number = 1;
			res_reverse = reverse_sum(step, update_ln, n);
			res = direct_sum(step, update_ln, n);
			res_cahon = cahon_sum(step, update_ln, n);
			printf("Result direct: %f Error:%f\n", res, fabsf(logf(1.0f + arg) - res));
			printf("Result reverse: %f Error:%f\n", res_reverse, fabsf(logf(1.0f + arg) - res_reverse));
			printf("Result cahon: %f Error:%f\n", res_cahon, fabsf(logf(1.0f + arg) - res_cahon));
			printf("Math.h: %f\n", logf(1.0f+arg));
			break;

		default:
			printf("wrong number");
			return 1;
	}
	return 0;
}
