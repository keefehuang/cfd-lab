#include "helper.h"
#include "visual.h"
#include <stdio.h>


void write_vtkFile(const char *outputFolder, const char *szProblem, int timeStepNumber, int mpiRank, double xlength,
                   double ylength, int xStart, int yStart, int imax, int jmax, double dx, double dy, double **U,
                   double **V, double **P)
{
    int i, j;
    char szFileName[80];
    char szFileFullPath[512];
    FILE *fp = NULL;
    sprintf(szFileName, "%s.R%d.%i.vtk", szProblem, mpiRank, timeStepNumber);
    sprintf(szFileFullPath, "%s/%s", outputFolder, szFileName);
    fp = fopen(szFileFullPath, "w");
    if (fp == NULL)
    {
        char szBuff[80];
        sprintf(szBuff, "Failed to open %s", szFileFullPath);
        THROW_ERROR(szBuff);
        return;
    }
    
    write_vtkHeader(fp, imax, jmax, dx, dy);
    write_vtkPointCoordinates(fp, xStart, yStart, imax, jmax, dx, dy);
    
    fprintf(fp, "POINT_DATA %i \n", (imax + 1) * (jmax + 1));
    
    fprintf(fp, "\n");
    fprintf(fp, "VECTORS velocity float\n");
    for (j = 0; j < jmax + 1; j++)
    {
        for (i = 0; i < imax + 1; i++)
        {
            fprintf(fp, "%f %f 0\n", (U[i + 1][j] + U[i + 1][j + 1]) * 0.5, (V[i][j + 1] + V[i + 1][j + 1]) * 0.5);
        }
    }
    
    fprintf(fp, "\n");
    fprintf(fp, "CELL_DATA %i \n", (imax * jmax));
    fprintf(fp, "SCALARS pressure float 1 \n");
    fprintf(fp, "LOOKUP_TABLE default \n");
    for (j = 1; j < jmax + 1; j++)
    {
        for (i = 1; i < imax + 1; i++)
        {
            fprintf(fp, "%f\n", P[i][j]);
        }
    }
    
    if (fclose(fp))
    {
        char szBuff[80];
        sprintf(szBuff, "Failed to close %s", szFileFullPath);
        THROW_ERROR(szBuff);
    }
}


void write_vtkHeader(FILE *fp, int imax, int jmax,
                     double dx, double dy)
{
    if (fp == NULL)
    {
        char szBuff[80];
        sprintf(szBuff, "Null pointer in write_vtkHeader");
        THROW_ERROR(szBuff);
        return;
    }
    
    fprintf(fp, "# vtk DataFile Version 2.0\n");
    fprintf(fp, "generated by CFD-lab course output (written by Tobias Neckel) \n");
    fprintf(fp, "ASCII\n");
    fprintf(fp, "\n");
    fprintf(fp, "DATASET STRUCTURED_GRID\n");
    fprintf(fp, "DIMENSIONS  %i %i 1 \n", imax + 1, jmax + 1);
    fprintf(fp, "POINTS %i float\n", (imax + 1) * (jmax + 1));
    fprintf(fp, "\n");
}


void write_vtkPointCoordinates(FILE *fp, int xStart, int yStart, int imax, int jmax, double dx, double dy)
{
    double originX = xStart*dx;
    double originY = yStart*dy;
    
    int i = 0;
    int j = 0;
    
    for (j = 0; j < jmax + 1; j++)
    {
        for (i = 0; i < imax + 1; i++)
        {
            fprintf(fp, "%f %f 0\n", originX + (i * dx), originY + (j * dy));
        }
    }
}

