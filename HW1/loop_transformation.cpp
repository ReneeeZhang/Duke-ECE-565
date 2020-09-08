/*************************************
 *             Original
 *************************************/
...
int a[N][4]; 
int rand_number = rand();
for (i = 0; i < 4; i++) {
	threshold = 2.0 * rand_number;
	for (j = 0; j < N; j++) {
		if (threshold < 4) {
			sum = sum + a[j][i];
		} else {
			sum = sum + a[j][i] + 1;
		}
	} 
}
...

/*************************************
 *      Loop Invariant Hoisting
 *************************************/
...
int a[N][4]; 
int rand_number = rand();
threshold = 2.0 * rand_number;
for (i = 0; i < 4; i++) {
	for (j = 0; j < N; j++) {
		if (threshold < 4) {
			sum = sum + a[j][i];
		} else {
			sum = sum + a[j][i] + 1;
		}
	} 
}
...

/*************************************
 *         Loop Unswitching
 *************************************/
...
int a[N][4]; 
int rand_number = rand();
threshold = 2.0 * rand_number;
if (threshold < 4) {
	for (i = 0; i < 4; i++) {
		for (j = 0; j < N; j++) {
			sum = sum + a[j][i];
		}
	}
} else {
	for (i = 0; i < 4; i++) {
		for (j = 0; j < N; j++) {
			sum = sum + a[j][i] + 1;
		}
	}
}
...

/*************************************
 *         Loop Interchange
 *************************************/
...
int a[N][4]; 
int rand_number = rand();
threshold = 2.0 * rand_number;
if (threshold < 4) {
	for (j = 0; j < N; j++){
		for (i = 0; i < 4; i++) {
			sum = sum + a[j][i];
		}
	}
} else {
	for (j = 0; j < N; j++){
		for (i = 0; i < 4; i++) {
			sum = sum + a[j][i];
		}
	}
}
...

/*************************************
 *         Loop Unrolling
 *************************************/
...
int a[N][4]; 
int rand_number = rand();
threshold = 2.0 * rand_number;
if (threshold < 4) {
	for (j = 0; j < N; j++){
		sum = sum + a[j][0];
		sum = sum + a[j][1];
		sum = sum + a[j][2];
		sum = sum + a[j][3];
	}
} else {
	for (j = 0; j < N; j++){
			sum = sum + a[j][0] + 1;
			sum = sum + a[j][1] + 1;
			sum = sum + a[j][2] + 1;
			sum = sum + a[j][3] + 1;
	}
}
...