#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//essentially, this converts the histogram to a cumulative-frequency graph
//to find the corresponding real_contrib value after we generated a random number
double cumulative(double real_contrib[], double percent[], int n)
{
    double cumul[n]; //for storing the cumulative frequencies
    int i; //to be used in for-loops

    cumul[0] = percent[0];
    for (i=1; i<n; i++) {
        cumul[i] = cumul[i-1] + percent[i]; //generate the cumulative frequencies
    }

    //generate a random value from 0 to the total cumulative frequency, i.e. 1
    double rand_num = (rand() / (double)RAND_MAX );

    //find the corresponding index that contains the random number generated
    int midpt;
    int left=0, right=n-1;
    while (left < right) {
        midpt = (left + right) / 2;
        (rand_num > cumul[midpt]) ? (left = midpt + 1) : (right = midpt);
    }

    return real_contrib[left];
}

//for calculating the grade (final assessment distribution)
void weight1(int n, double assessments[n][n], double weighting1[n])
{
    int i, j; //to be used in for-loops

    //individual effort rating (IER) is the sum of the ratings given to a certain student from himself and his groupmates
    double IER[n];
    //average effort rating (AER) is the average IER in a group
    double AER = 0;
    //individual weighting factor (IWF) of a student is his IER divided by AER
    double IWF[n];

    //initialising these arrays to 0
    for (i=0; i<n; i++) {
        IER[i] = 0;
        IWF[i] = 0;
    }

    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            IER[i] += assessments[j][i];
        }
    //printf("\nIER of student %d = %lf", i+1, IER[i]);
    }

    for (i=0; i<n; i++) {
        AER += IER[i];
    }
    AER /= (double)n;
    //printf("\nAER = %lf", AER);

    double sum=0;
    for (i=0; i<n; i++) {
        IWF[i] = IER[i] / AER;
        sum += IWF[i];
        //printf("\nIWF of student %d = %lf", i+1, IWF[i]);
    }

    for (i=0; i<n; i++) {
        weighting1[i] = 4.0/sum * IWF[i]; //normalise such that the sum of the assessments are equal to 4
    }
}

//calculate grade using weight1 but ignoring self-assessments this time
void weight2(int n, double assessments[n][n], double weighting2[n])
{
    int i, j; //to be used in for-loops

    //individual effort rating (IER) is the sum of the ratings given to a certain student from himself and his groupmates
    double IER[n];
    //average effort rating (AER) is the average IER in a group
    double AER = 0;
    //individual weighting factor (IWF) of a student is his IER divided by AER
    double IWF[n];

    //initialising these arrays to 0
    for (i=0; i<n; i++) {
        IER[i] = 0;
        IWF[i] = 0;
    }

    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            if (i != j)
                IER[i] += assessments[j][i];
        }
    //printf("\nIER of student %d = %lf", i+1, IER[i]);
    }

    for (i=0; i<n; i++) {
        AER += IER[i];
    }
    AER /= (double)n;
    //printf("\nAER = %lf", AER);

    double sum=0;
    for (i=0; i<n; i++) {
        IWF[i] = IER[i] / AER;
        sum += IWF[i];
        //printf("\nIWF of student %d = %lf", i+1, IWF[i]);
    }

    for (i=0; i<n; i++) {
        weighting2[i] = 4.0/sum * IWF[i]; //normalise such that the sum of the assessments are equal to 4
    }
}


//calculate grade with method 5
void weight5(int n, double assessments[n][n], double weighting5[n], int ac_or_AS)
{
    int i, j; //to be used in for-loops

    //normalised individual weighting factor (NIWF) of a student is the sum of assessments given to him
    double NIWF[n];
    //assessee standard deviation (s) calculated from NIWF
    double s[n];
    double max_s=0; //for storing the maximum standard deviation
    double IAF[n]; //IAF = 1 - s/s_max
    //individual weighting factor (IWF)
    double IWF[n]; //IWF = IAF * (NIWF - 1) +1

    //initialising these arrays to 0
    for (i=0; i<n; i++) {
        NIWF[i] = 0;
        s[i] = 0;
        IAF[i] = 0;
        IWF[i] = 0;
    }

    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            NIWF[i] += assessments[j][i];
        }
    }

    //find standard deviation
    for (i=0; i<n; i++) {
        double mean = NIWF[i] / 4.0;
        for (j=0; j<n; j++) {
            s[i] += (assessments[j][i] - mean) * (assessments[j][i] - mean);
        }
        s[i] = sqrt(s[i]/4.0);
    }

    //find the maximum standard deviation
    for (i=0; i<n; i++) {
        if (s[i] > max_s)
            max_s = s[i];
    }

    double sum=0;
    for (i=0; i<n; i++) {
        if (ac_or_AS == 0) {
            IAF[i] = 1 - s[i]/max_s;
            IWF[i] = IAF[i] * (NIWF[i] - 1) + 1;
            sum+=IWF[i];
        } else if (ac_or_AS == 1) {
            IAF[i] = 1 - s[i]/(2*max_s);
            IWF[i] = IAF[i] * (NIWF[i] - 1) + 1;
            sum+=IWF[i];
        }
    }

    for (i=0; i<n; i++) {
        weighting5[i] = 4.0/sum * IWF[i]; //normalise such that the sum of the assessments are equal to 4
    }
}

//same as weight5 but without self-assessments
void weight7(int n, double assessments[n][n], double weighting7[n], int ac_or_AS)
{
    int i, j; //to be used in for-loops

    //normalised individual weighting factor (NIWF) of a student is the sum of assessments given to him
    double NIWF[n];
    //assessee standard deviation (s) calculated from NIWF
    double s[n];
    double max_s=0; //for storing the maximum standard deviation
    double IAF[n]; //IAF = 1 - s/s_max
    //individual weighting factor (IWF)
    double IWF[n]; //IWF = IAF * (NIWF - 1) +1

    //initialising these arrays to 0
    for (i=0; i<n; i++) {
        NIWF[i] = 0;
        s[i] = 0;
        IAF[i] = 0;
        IWF[i] = 0;
    }

    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            if (i != j)
                NIWF[i] += assessments[j][i];
        }
    }

    //find standard deviation
    for (i=0; i<n; i++) {
        double mean = NIWF[i] / 3.0;
        for (j=0; j<n; j++) {
            s[i] += (assessments[j][i] - mean) * (assessments[j][i] - mean);
        }
        s[i] = sqrt(s[i]/3.0);
    }

    //find the maximum standard deviation
    for (i=0; i<n; i++) {
        if (s[i] > max_s)
            max_s = s[i];
    }

    double sum=0;
    for (i=0; i<n; i++) {
        if (ac_or_AS == 0) {
            IAF[i] = 1 - s[i]/max_s;
            IWF[i] = IAF[i] * (NIWF[i] - 1) + 1;
            sum+=IWF[i];
        } else if (ac_or_AS == 1) {
            IAF[i] = 1 - s[i]/(2*max_s);
            IWF[i] = IAF[i] * (NIWF[i] - 1) + 1;
            sum+=IWF[i];
        }
    }

    for (i=0; i<n; i++) {
        weighting7[i] = 4.0/sum * IWF[i]; //normalise such that the sum of the assessments are equal to 4
    }
}


double error(double old_value, double new_value)
{
    return (new_value - old_value) / old_value;
}


int main()
{
    //the real contributions of students with 1 = 100%
    //these two arrays are the samples given in the literature
    //we use it as a probability distribution to generate real contribution values with the Monte Carlo method
    double real_contrib[] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};
    //the distribution for each of the real contributions
    double percent[] = {0.0001, 0.0002, 0.0003, 0.0005, 0.001, 0.0025, 0.005, 0.015, 0.045, 0.15, 0.565,
                        0.15, 0.045, 0.015, 0.005, 0.0025, 0.001, 0.0005, 0.0003, 0.0002, 0.0001};

    int i, j, k;  //to be used in for-loops
    int n = sizeof(real_contrib) / sizeof(real_contrib[0]); //size of real_contrib array

    //the total number of groups
    int trials = 500;
    //the number of people in each group
    int no_of_groupmate = 4;
    //an array for storing the real contributions for each person
    double random_real_contrib[trials][no_of_groupmate];
    //an array for storing the assessments each group member gives to each other
    double assessments[trials][no_of_groupmate][no_of_groupmate];
    //an array for storing the final grades of each student
    double grades[trials][no_of_groupmate];

    srand(time(NULL));

    FILE *fp1 = fopen("Gradings.txt", "w");
    fprintf(fp1, "Trial no.\tMethod 1\tMethod 2\tMethod 3\tMethod 4\tMethod 5\tMethod 6\tMethod 7\tMethod 8\tSelf inflation");

    FILE *fp2 = fopen("Error vs contrib.txt", "w");
    fprintf(fp2, "Trial no.\tMethod 1\tMethod 2\tMethod 3\tMethod 4\tMethod 5\tMethod 6\tMethod 7\tMethod 8\tReal contrib");

    FILE *fp3 = fopen("Table.txt", "w");
    fprintf(fp3, "\t\t\t\t\t\t\tAssessee (j)");
    fprintf(fp3, "\n\t\t\t\t\t\tA\tB\tC\tD");
    fprintf(fp3, "\nOriginal raw assessments (RA)\nAssessor (i)");


    //now we generate n random numbers
    for (i=0; i<trials; i++) {
        double sum=0;
        //printf("\n\n\n##################Group %d##################", i+1);

        for (j=0; j<no_of_groupmate; j++) {
            //call the function to generate random real contributions
            random_real_contrib[i][j] = cumulative(real_contrib, percent, n);
            sum+=random_real_contrib[i][j];
        }

        for (j=0; j<no_of_groupmate; j++) {
            //since a real contribution of 1 is 100%, which means the average contribution and fair share of the group's work
            //the sum of all 4 contributions should be 4, i.e. 400%
            //the step below is "normalising" the real contribution values such that the sum = 4
            random_real_contrib[i][j] *= (double)no_of_groupmate / sum;
            //printf("\nReal contribution of student %d = %lf", j+1, random_real_contrib[i][j]);
        }

        double inflation=0;

        for (j=0; j<no_of_groupmate; j++) {
            //printf("\n");
            //suppose the assessments each member gives to others are within 10% of the real contributions
            //we randomly generate assessment values within 10% of the real contributions
            //UNLESS it is a self-assessment, as students tend to inflate their own contribution

            for (k=0; k<no_of_groupmate; k++) {
                double random;
                if (k == j) {
                    //from literature, the average self-assessment inflation is 21.45%
                    //generate random number between 0 to 41.29% (i.e. 21.45% x 2)
                    random = ((rand() / (double)RAND_MAX ) * 4.129) / 10.0;
                    inflation += random;
                } else {
                    //generate random number between -0.1 to 0.1
                    random = ((rand() / (double)RAND_MAX ) * 2.0 - 1.0) / 10.0;
                }
                //printf("\nRandom = %lf", random);

                assessments[i][j][k] = random_real_contrib[i][k] * (1+random);
                //printf("\nAssessents of student %d on student %d = %lf", j+1, k+1, assessments[i][j][k]);
            }

        }

        if (i == 1) {
            fprintf(fp3, "\nA\t\t\t\t\t\t%.2lf\t%.2lf\t%.2lf\t%.2lf",
                   assessments[i][0][0], assessments[i][0][1], assessments[i][0][2], assessments[i][0][3]);
            fprintf(fp3, "\nB\t\t\t\t\t\t%.2lf\t%.2lf\t%.2lf\t%.2lf",
                   assessments[i][1][0], assessments[i][1][1], assessments[i][1][2], assessments[i][1][3]);
            fprintf(fp3, "\nC\t\t\t\t\t\t%.2lf\t%.2lf\t%.2lf\t%.2lf",
                   assessments[i][2][0], assessments[i][2][1], assessments[i][2][2], assessments[i][2][3]);
            fprintf(fp3, "\nD\t\t\t\t\t\t%.2lf\t%.2lf\t%.2lf\t%.2lf",
                   assessments[i][3][0], assessments[i][3][1], assessments[i][3][2], assessments[i][3][3]);
        }


        //##################WEIGHTING 1##################
        //this part is for calculating the weighting (final grade distribution) of the student)
        double weighting1[no_of_groupmate];
        double error1[no_of_groupmate];
        double error_sum1=0;
        weight1(no_of_groupmate, assessments[i], weighting1);
        //print the results
        //printf("\nUsing weighting method 1:");
        for (j=0; j<no_of_groupmate; j++) {
            //printf("\nGrade of student %d = %lf", j+1, weighting1[j]);
            error1[j] = error(random_real_contrib[i][j], weighting1[j]);
            error_sum1 += error1[j];
            //printf(", with error of %lf%%", error1[j]);
        }
        //printf("\nAverage error = %lf%%", error_sum1 / 4.0);

        //##################WEIGHTING 2##################
        //we use the weighting above but ignore self-assessments
        double weighting2[no_of_groupmate];
        double error2[no_of_groupmate];
        double error_sum2=0;
        weight2(no_of_groupmate, assessments[i], weighting2);
        //print the results
        //printf("\n\nUsing weighting method 2:");
        for (j=0; j<no_of_groupmate; j++) {
            //printf("\nGrade of student %d = %lf", j+1, weighting2[j]);
            error2[j] = error(random_real_contrib[i][j], weighting2[j]);
            error_sum2 += error2[j];
            //printf(", with error of %lf%%", error2[j]);
        }
        //printf("\nAverage error = %lf%%", error_sum2 / 4.0);


        //##################WEIGHTING 3##################
        //this time, we normalise the assessments first (with self-assessments included)
        double weighting3[no_of_groupmate];
        double error3[no_of_groupmate];
        double error_sum3=0;
        double sum3[no_of_groupmate];
        //perform normalisation
        for (j=0; j<no_of_groupmate; j++) {
            sum3[j] = 0;
            for (k=0; k<no_of_groupmate; k++) {
                sum3[j] += assessments[i][j][k];
            }
        }
        for (j=0; j<no_of_groupmate; j++) {
            for (k=0; k<no_of_groupmate; k++) {
                assessments[i][j][k] = 1.0/sum3[j] * assessments[i][j][k];
            }
        }

        if (i == 1) {
            fprintf(fp3, "\n\nNormalised assessments (NAs)");
            fprintf(fp3, "\nA\t\t\t\t\t\t%.2lf\t%.2lf\t%.2lf\t%.2lf",
                   assessments[i][0][0], assessments[i][0][1], assessments[i][0][2], assessments[i][0][3]);
            fprintf(fp3, "\nB\t\t\t\t\t\t%.2lf\t%.2lf\t%.2lf\t%.2lf",
                   assessments[i][1][0], assessments[i][1][1], assessments[i][1][2], assessments[i][1][3]);
            fprintf(fp3, "\nC\t\t\t\t\t\t%.2lf\t%.2lf\t%.2lf\t%.2lf",
                   assessments[i][2][0], assessments[i][2][1], assessments[i][2][2], assessments[i][2][3]);
            fprintf(fp3, "\nD\t\t\t\t\t\t%.2lf\t%.2lf\t%.2lf\t%.2lf",
                   assessments[i][3][0], assessments[i][3][1], assessments[i][3][2], assessments[i][3][3]);
        }

        weight1(no_of_groupmate, assessments[i], weighting3);
        //print the results
        //printf("\n\nUsing weighting method 3:");
        for (j=0; j<no_of_groupmate; j++) {
            //printf("\nGrade of student %d = %lf", j+1, weighting3[j]);
            error3[j] = error(random_real_contrib[i][j], weighting3[j]);
            error_sum3 += error3[j];
            //printf(", with error of %lf%%", error3[j]);
        }
        //printf("\nAverage error = %lf%%", error_sum3 / 4.0);


        //##################WEIGHTING 4##################
        //use normalised assessments but with ac-IWF computation using NAs
        double weighting4[no_of_groupmate];
        double error4[no_of_groupmate];
        double error_sum4=0;
        double sum4[no_of_groupmate];
        weight5(no_of_groupmate, assessments[i], weighting4, 0);
        //print the results
        //printf("\n\nUsing weighting method 4:");
        for (j=0; j<no_of_groupmate; j++) {
            //printf("\nGrade of student %d = %lf", j+1, weighting4[j]);
            error4[j] = error(random_real_contrib[i][j], weighting4[j]);
            error_sum4 += error4[j];
            //printf(", with error of %lf%%", error4[j]);
        }
        //printf("\nAverage error = %lf%%", error_sum4 / 4.0);


        //##################WEIGHTING 5##################
        //use normalised assessments but with ASNIWF computations using NAs
        double weighting5[no_of_groupmate];
        double error5[no_of_groupmate];
        double error_sum5=0;
        double sum5[no_of_groupmate];
        weight5(no_of_groupmate, assessments[i], weighting5, 1);
        //print the results
        //printf("\n\nUsing weighting method 5:");
        for (j=0; j<no_of_groupmate; j++) {
            //printf("\nGrade of student %d = %lf", j+1, weighting5[j]);
            error5[j] = error(random_real_contrib[i][j], weighting5[j]);
            error_sum5 += error5[j];
            //printf(", with error of %lf%%", error5[j]);
        }
        //printf("\nAverage error = %lf%%", error_sum5 / 4.0);


        //##################WEIGHTING 6##################
        //normalised assessments without self-assessment
        double weighting6[no_of_groupmate];
        double error6[no_of_groupmate];
        double error_sum6=0;
        double sum6[no_of_groupmate];
        //perform normalisation without self-assessment
        for (j=0; j<no_of_groupmate; j++) {
            sum6[j] = 0;
            for (k=0; k<no_of_groupmate; k++) {
                if (j == k)
                    assessments[i][j][k] = 0;
                sum6[j] += assessments[i][j][k];
            }
        }
        for (j=0; j<no_of_groupmate; j++) {
            for (k=0; k<no_of_groupmate; k++) {
                assessments[i][j][k] = 1.0/sum6[j] * assessments[i][j][k];
            }
        }

        if (i == 1) {
            fprintf(fp3, "\n\nNormalised assessments (NAs) without self-assessments");
            fprintf(fp3, "\nA\t\t\t\t\t\t\t%.2lf\t%.2lf\t%.2lf",
                   assessments[i][0][1], assessments[i][0][2], assessments[i][0][3]);
            fprintf(fp3, "\nB\t\t\t\t\t\t%.2lf\t\t%.2lf\t%.2lf",
                   assessments[i][1][0], assessments[i][1][2], assessments[i][1][3]);
            fprintf(fp3, "\nC\t\t\t\t\t\t%.2lf\t%.2lf\t\t%.2lf",
                   assessments[i][2][0], assessments[i][2][1], assessments[i][2][3]);
            fprintf(fp3, "\nD\t\t\t\t\t\t%.2lf\t%.2lf\t%.2lf",
                   assessments[i][3][0], assessments[i][3][1], assessments[i][3][2]);
        }

        weight2(no_of_groupmate, assessments[i], weighting6);
        //print the results
        //printf("\n\nUsing weighting method 6:");
        for (j=0; j<no_of_groupmate; j++) {
            //printf("\nGrade of student %d = %lf", j+1, weighting6[j]);
            error6[j] = error(random_real_contrib[i][j], weighting6[j]);
            error_sum6 += error6[j];
            //printf(", with error of %lf%%", error6[j]);
        }
        //printf("\nAverage error = %lf%%", error_sum6 / 4.0);


        //##################WEIGHTING 7##################
        //use normalised assessments but with ac-IWF-esa computation using NAs-esa
        double weighting7[no_of_groupmate];
        double error7[no_of_groupmate];
        double error_sum7=0;
        double sum7[no_of_groupmate];
        weight7(no_of_groupmate, assessments[i], weighting7, 0);
        //print the results
        //printf("\n\nUsing weighting method 7:");
        for (j=0; j<no_of_groupmate; j++) {
            //printf("\nGrade of student %d = %lf", j+1, weighting7[j]);
            error7[j] = error(random_real_contrib[i][j], weighting7[j]);
            error_sum7 += error7[j];
            //printf(", with error of %lf%%", error7[j]);
        }
        //printf("\nAverage error = %lf%%", error_sum7 / 4.0);


        //##################WEIGHTING 8##################
        //use normalised assessments but with ASNIWF-esa computations using NAs-esa
        double weighting8[no_of_groupmate];
        double error8[no_of_groupmate];
        double error_sum8=0;
        double sum8[no_of_groupmate];
        weight7(no_of_groupmate, assessments[i], weighting8, 1);
        //print the results
        //printf("\n\nUsing weighting method 8:");
        for (j=0; j<no_of_groupmate; j++) {
            //printf("\nGrade of student %d = %lf", j+1, weighting8[j]);
            error8[j] = error(random_real_contrib[i][j], weighting8[j]);
            error_sum8 += error8[j];
            //printf(", with error of %lf%%", error8[j]);
        }
        //printf("\nAverage error = %lf%%", error_sum8 / 4.0);




        //print out the errors to be used in Excel
        fprintf(fp1, "\n%d\t\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf", i,
                error_sum1/4.0, error_sum2/4.0, error_sum3/4.0, error_sum4/4.0,
                error_sum5/4.0, error_sum6/4.0, error_sum7/4.0, error_sum8/4.0,
                inflation/4.0);

        for (j=0; j<no_of_groupmate; j++) {
            fprintf(fp2, "\n%d\t\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf", j,
                    error1[j], error2[j], error3[j], error4[j],
                    error5[j], error6[j], error7[j], error8[j],
                    random_real_contrib[i][j]);
        }

    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;
}
