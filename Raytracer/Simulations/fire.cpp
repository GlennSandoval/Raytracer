#include "fire.h"
#include <iostream>
#include <fstream>

using namespace std;

Fire::Fire()
{}

Fire::Fire(int x) {
    N = x;
	dim = N+2;
	size = dim * dim * dim;
	dt = 0.1;

	visc = 0.0;
	diff = 0.0;

	burnRate = 10;
	ignitionTemp = 1000;

	r = 5;
	startVel = 1;

	d.resize(size);
	dOld.resize(size);

	u.resize(size);
	uOld.resize(size);

	v.resize(size);
	vOld.resize(size);

	w.resize(size);
	wOld.resize(size);

	t.resize(size);
	tOld.resize(size);
	
	fuel.resize(size);
	fuelOld.resize(size);

	su.resize(size);
	suOld.resize(size);

	sv.resize(size);
	svOld.resize(size);

	sw.resize(size);
	swOld.resize(size);

	st.resize(size);
	stOld.resize(size);

	vorticity = false;

	// initialize all values to 0
	clear();

	int cx = dim / 2;
	int cy = 2;
	int cz = dim / 2;

	//fuel[I(cx,cy,cz)] = 10.0;
	for (int i=0; i < dim; i++) {
        for (int j=0; j < dim; j++) {
			for (int k = 0; k < dim; k++) {
				if (j == cy) {
					float dist = abs(sqrt((float)(i-cx) * (i-cx) + (k-cz) * (k-cz)));
					if (dist < r) {
						fuel[I(i,j,k)] = 10.0;
					}
				}
			}
		}
	}
}

Fire::~Fire()
{}

int Fire::I(int x, int y, int z) {
    return (z * dim * dim) + (y * dim) + x;
}

void Fire::clear() {
    for (int i=0; i < size; i++) {
        u[i] = uOld[i] = v[i] = vOld[i] = w[i] = wOld[i] = d[i] = dOld[i] = 0.0;
		fuel[i] = fuelOld[i] = 0.0;
    }
}

void Fire::addSource( vector<float>& arr, vector<float>& arrOld )
{
	for (int i=0; i < size; i++ ) {
        arr[i] += dt * arrOld[i];
    }
}

void Fire::boundary( int type, vector<float>& arr )
{
	// side borders
	for (int i=1; i <= N; i++ ) {
		switch (type) {
            case (HORIZ):
                arr[I(0,i,i)] = -arr[I(1,i,i)];
                arr[I(N+1,i,i)] = -arr[I(N,i,i)];
                break;
            case (VERT):
                arr[I(i,0,i)] = -arr[I(i,1,i)];
                arr[I(i,N+1,i)] = -arr[I(i,N,i)];
                break;
			case (DEPTH):
				arr[I(i,i,0)] = -arr[I(i,i,1)];
				arr[I(i,i,N+1)] = -arr[I(i,i,N)];
				break;
            default:
                arr[I(0,i,i)] = arr[I(1,i,i)];
                arr[I(N+1,i,i)] = arr[I(N,i,i)];
                arr[I(i,0,i)] = arr[I(i,1,i)]; 
                arr[I(i,N+1,i)] = arr[I(i,N,i)];
				arr[I(i,i,0)] = arr[I(i,i,1)];
				arr[I(i,i,N+1)] = arr[I(i,i,N)];
                break;
        }
    }
	// corners
    arr[I(0,0,0)] = 0.33f * (arr[I(1,0,0)] + arr[I(0,1,0)] + arr[I(0,0,1)]); 
    arr[I(0,N+1,0)] = 0.33f * (arr[I(1,N+1,0)] + arr[I(0,N,0)] + arr[I(0,N+1,1)]);
    arr[I(N+1,0,0)] = 0.33f * (arr[I(N,0,0)] + arr[I(N+1,1,0)] + arr[I(N+1,0,1)]);   
    arr[I(N+1,N+1,0)] = 0.33f * (arr[I(N,N+1,0)] + arr[I(N+1,N,0)] + arr[I(N+1,N+1,1)]); 

	arr[I(0,0,N+1)] = 0.33f * (arr[I(1,0,N+1)] + arr[I(0,1,N+1)] + arr[I(0,0,N)]);   
    arr[I(0,N+1,N+1)] = 0.33f * (arr[I(1,N+1,N+1)] + arr[I(0,N,N+1)] + arr[I(0,N+1,N)]);   
    arr[I(N+1,0,N+1)] = 0.33f * (arr[I(N,0,N+1)] + arr[I(N+1,1,N+1)] + arr[I(N+1,0,N)]);   
    arr[I(N+1,N+1,N+1)] = 0.33f * (arr[I(N,N+1,N+1)] + arr[I(N+1,N,N+1)] + arr[I(N+1,N+1,N)]); 
}

void Fire::linearSolver(int type, vector<float> &arr, vector<float> &arrOld, float a, float c)
{
	for (int m=0; m<20; m++ ) {
		for (int i=1; i <= N; i++) {
            for (int j=1; j <= N; j++) {
				for (int k=1; k <= N; k++) {
					arr[I(i,j,k)] = (arrOld[I(i,j,k)] + a * (arr[I(i-1,j,k)] + arr[I(i+1,j,k)] + 
                                                             arr[I(i,j-1,k)] + arr[I(i,j+1,k)] + 
														     arr[I(i,j,k-1)] + arr[I(i,j,k+1)])) / c;
				}
            }
        }
		boundary( type, arr );
	}
}

void Fire::diffuse(int type, vector<float> &arr, vector<float> &arrOld, float dif)
{
	float a = dt * dif * N * N;
	linearSolver( type, arr, arrOld, a, 1+6*a );
}

void Fire::backtrace(int type, vector<float> &arr, vector<float> &arrOld, vector<float> velX, vector<float> velY, vector<float> velZ)
{
	for (int i=1; i <= N; i++) {
		for (int j=1; j <= N; j++) {
			for (int k=1; k <= N; k++) {
				float newI = i - dt * velX[I(i,j,k)];
				float newJ = j - dt * velY[I(i,j,k)];
				float newK = k - dt * velZ[I(i,j,k)];

				int x = (int)newI;
				int y = (int)newJ;
				int z = (int)newK;

				float dx = newI - x; 
				float dy = newJ - y; 
				float dz = newK - z;

				arr[I(i,j,k)] = (1-dx) * (1-dy) * (1-dz) * arrOld[I(  x,  y,  z)] +
								(1-dx) *    dy  * (1-dz) * arrOld[I(  x,y+1,  z)] +
								   dx  * (1-dy) * (1-dz) * arrOld[I(x+1,  y,  z)] +
								   dx  *    dy  * (1-dz) * arrOld[I(x+1,y+1,  z)] +
								(1-dx) * (1-dy) *    dz  * arrOld[I(  x,  y,z+1)] +
								(1-dx) *    dy  *    dz  * arrOld[I(  x,y+1,z+1)] +
								   dx  * (1-dy) *    dz  * arrOld[I(x+1,  y,z+1)] +
								   dx  *    dy  *    dz  * arrOld[I(x+1,y+1,z+1)];
			}
		}
	}
    boundary( type, arr );  
}

void Fire::cool(int type, vector<float> &arr, vector<float> &arrOld, vector<float> &temp, vector<float> &tempOld, vector<float> velX, vector<float> velY, vector<float> velZ) {
	for (int i=1; i <= N; i++) {
		for (int j=1; j <= N; j++) {
			for (int k=1; k <= N; k++) {
				float newI = i - dt * velX[I(i,j,k)];
				float newJ = j - dt * velY[I(i,j,k)];
				float newK = k - dt * velZ[I(i,j,k)];

				int x = (int)newI;
				int y = (int)newJ;
				int z = (int)newK;

				float dx = newI - x; 
				float dy = newJ - y; 
				float dz = newK - z;

				arr[I(i,j,k)] = (1-dx) * (1-dy) * (1-dz) * arrOld[I(  x,  y,  z)] +
								(1-dx) *    dy  * (1-dz) * arrOld[I(  x,y+1,  z)] +
								   dx  * (1-dy) * (1-dz) * arrOld[I(x+1,  y,  z)] +
								   dx  *    dy  * (1-dz) * arrOld[I(x+1,y+1,  z)] +
								(1-dx) * (1-dy) *    dz  * arrOld[I(  x,  y,z+1)] +
								(1-dx) *    dy  *    dz  * arrOld[I(  x,y+1,z+1)] +
								   dx  * (1-dy) *    dz  * arrOld[I(x+1,  y,z+1)] +
								   dx  *    dy  *    dz  * arrOld[I(x+1,y+1,z+1)];
				temp[I(i,j,k)] = (1-dx) * (1-dy) * (1-dz) * tempOld[I(  x,  y,  z)] +
								 (1-dx) *    dy  * (1-dz) * tempOld[I(  x,y+1,  z)] +
								    dx  * (1-dy) * (1-dz) * tempOld[I(x+1,  y,  z)] +
								    dx  *    dy  * (1-dz) * tempOld[I(x+1,y+1,  z)] +
								 (1-dx) * (1-dy) *    dz  * tempOld[I(  x,  y,z+1)] +
								 (1-dx) *    dy  *    dz  * tempOld[I(  x,y+1,z+1)] +
								    dx  * (1-dy) *    dz  * tempOld[I(x+1,  y,z+1)] +
								    dx  *    dy  *    dz  * tempOld[I(x+1,y+1,z+1)];
				temp[I(i,j,k)] *= 0.9;
			}
		}
	}
    boundary( type, arr );  
}

void Fire::project( vector<float>& p, vector<float>& div )
{
	for (int i=1; i <= N; i++) {
		for (int j=1; j <= N; j++) {
			for (int k=1; k <= N; k++) {
				float dx = 0.5 * (u[I(i+1,j,k)] - u[I(i-1,j,k)]);
				float dy = 0.5 * (v[I(i,j+1,k)] - v[I(i,j-1,k)]);
				float dz = 0.5 * (w[I(i,j,k+1)] - w[I(i,j,k-1)]);
				div[I(i,j,k)] = (dx + dy + dz); 
				p[I(i,j,k)] = 0;   
			}
		}
	}  
    boundary( CORNERS, div ); 
	boundary( CORNERS, p );   
   
    linearSolver( 0, p, div, 1, 6 );   
   
    for (int i=1; i <= N; i++) {
		for (int j=1; j <= N; j++) {
			for (int k=1; k <= N; k++) {
				float dx = 0.5 * (p[I(i+1,j,k)] - p[I(i-1,j,k)]);
				float dy = 0.5 * (p[I(i,j+1,k)] - p[I(i,j-1,k)]);
				float dz = 0.5 * (p[I(i,j,k+1)] - p[I(i,j,k-1)]);
				u[I(i,j,k)] += dx;   
				v[I(i,j,k)] += dy;
				w[I(i,j,k)] += dz;
			}
		}
	}
    boundary( HORIZ, u ); 
	boundary( VERT, v );
	boundary( DEPTH, w );
}

// make the steam rise
void Fire::buoyancy(vector<float>& arr) {
   // float temp = 0;

   // // Find average temperature
   // for (int i=1; i <= N; i++) {
   //     for (int j=1; j <= N; j++) {
			//for (int k=1; k <= N; k++) {
			//	temp += t[I(i,j,k)];
			//}
   //     }
   // }
   // temp /= (N * N * N);

    // Add force in the y direction
    for (int i=1; i <= N; i++) {
        for (int j=1; j <= N; j++) {
			for (int k=1; k <= N; k++) {
				arr[I(i,j,k)] = 0.005 * t[I(i,j,k)];
				//arr[I(i,j,k)] = 0.0005 * fuel[I(i,j,k)] + 0.0025 * (fuel[I(i,j,k)] - temp);
			}
        }
    }
}


/*
 * curl
 * w = del x u = (duy/dx - dux/dy)
 */
vector<float> Fire::curl(int i, int j, int k) {
	float dudy = 0.5 * (u[I(i,j+1,k)] - u[I(i,j-1,k)]);
	float dudz = 0.5 * (u[I(i,j,k+1)] - u[I(i,j,k-1)]);

	float dvdx = 0.5 * (v[I(i+1,j,k)] - v[I(i-1,j,k)]);
	float dvdz = 0.5 * (v[I(i,j,k+1)] - v[I(i,j,k-1)]);

	float dwdx = 0.5 * (w[I(i+1,j,k)] - w[I(i-1,j,k)]);
	float dwdy = 0.5 * (w[I(i,j+1,k)] - w[I(i,j-1,k)]);

	vector<float> result = vector<float>(3);
	result[0] = dwdy - dvdz;
	result[1] = dudz - dwdx;
	result[2] = dvdx - dudy;

	return result;
}

/*
 * vorticity confinement
 * w = del x u (curl)
 * N = del |w| / | del |w| | (gradients)
 */
void Fire::vorticityConfinement(vector<float>& uVel, vector<float>& vVel, vector<float>& wVel)
{
	for (int i=2; i < N; i++) {
        for (int j=2; j < N; j++) {
			for (int k=2; k < N; k++) {
				vector<float> nwx1 = curl(i + 1, j, k);
				vector<float> nwx2 = curl(i - 1, j, k);
				vector<float> nwxdiff = vector<float>(3);
				nwxdiff[0] = nwx1[0] - nwx2[0];
				nwxdiff[1] = nwx1[1] - nwx2[1];
				nwxdiff[2] = nwx1[2] - nwx2[2];
				float nwx = 0.5 * sqrt(nwxdiff[0] * nwxdiff[0] + 
									   nwxdiff[1] * nwxdiff[1] + 
									   nwxdiff[2] * nwxdiff[2]);

				vector<float> nwy1 = curl(i, j + 1, k);
				vector<float> nwy2 = curl(i, j - 1, k);
				vector<float> nwydiff = vector<float>(3);
				nwydiff[0] = nwy1[0] - nwy2[0];
				nwydiff[1] = nwy1[1] - nwy2[1];
				nwydiff[2] = nwy1[2] - nwy2[2];
				float nwy = 0.5 * sqrt(nwydiff[0] * nwydiff[0] + 
									   nwydiff[1] * nwydiff[1] + 
									   nwydiff[2] * nwydiff[2]);

				vector<float> nwz1 = curl(i, j, k + 1);
				vector<float> nwz2 = curl(i, j, k - 1);
				vector<float> nwzdiff = vector<float>(3);
				nwzdiff[0] = nwz1[0] - nwz2[0];
				nwzdiff[1] = nwz1[1] - nwz2[1];
				nwzdiff[2] = nwz1[2] - nwz2[2];
				float nwz = 0.5 * sqrt(nwzdiff[0] * nwzdiff[0] + 
									   nwzdiff[1] * nwzdiff[1] + 
									   nwzdiff[2] * nwzdiff[2]);

				// N = n / |n|
				float mag = sqrt(nwx * nwx + nwy * nwy + nwz * nwz);
				nwx /= mag;
				nwy /= mag;
				nwz /= mag;

				vector<float> temp = curl(i,j,k);
				uVel[I(i,j,k)] = nwy * temp[2] - temp[1] * nwz;
				vVel[I(i,j,k)] = nwz * temp[0] - temp[2] * nwx;
				wVel[I(i,j,k)] = nwx * temp[1] - temp[0] * nwy; 
			}
        }
    }
}

void Fire::burnFuel() {
	for (int i=0; i < size; i++) {
		if (t[i] > ignitionTemp && fuel[i] > 0) {
			float deltaF = burnRate * dt;
			fuel[i] -= deltaF; // reduce amount of fuel by burn rate * dt
			if (fuel[i] < 0) fuel[i] = 0; // clamp to 0
			d[i] += 0.05 * deltaF; // add s * delta F to smoke concentration
			t[i] += 10 * deltaF;
		}
	}
}

void Fire::densityStep()
{
	//addSource( d, dOld );

    swap( dOld, d );
	diffuse( CORNERS, d, dOld, diff );
    
	swap( dOld, d );
	backtrace( CORNERS, d, dOld, u, v, w );
}

void Fire::tempStep()
{
	addSource( fuel, fuelOld );
	addSource( t, tOld );

	swap( fuelOld, fuel );
	diffuse( CORNERS, fuel, fuelOld, diff );

	swap( tOld, t );
	diffuse( CORNERS, t, tOld, diff );

	swap( fuelOld, fuel );
	swap( tOld, t );

	cool( CORNERS, fuel, fuelOld, t, tOld, u, v, w );

	burnFuel();
}

void Fire::velocityStep()
{
	addSource( u, uOld ); 
	addSource( v, vOld );
	addSource( w, wOld );

	if (vorticity) {
		vorticityConfinement(uOld, vOld, wOld);
		addSource( u, uOld );
		addSource( v, vOld );
		addSource( w, wOld );
	}

    buoyancy(vOld);
    addSource( v, vOld );
    
	swap( uOld, u ); 
	diffuse( HORIZ, u, uOld, visc );   
    swap( vOld, v ); 
	diffuse( VERT, v, vOld, visc );
	swap( wOld, w );
	diffuse( DEPTH, w, wOld, visc );
    
	project( uOld, vOld );   
    swap( uOld, u ); 
	swap( vOld, v );
	swap( wOld, w );

    backtrace( HORIZ, u, uOld, uOld, vOld, wOld ); 
	backtrace( VERT, v, vOld, uOld, vOld, wOld );
	backtrace( DEPTH, w, wOld, uOld, vOld, wOld );
    project( uOld, vOld );
}
