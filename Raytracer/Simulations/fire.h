#ifndef __Fire__
#define __Fire__

#include <vector>

using namespace std;

#define CORNERS 0
#define HORIZ   1
#define VERT    2
#define DEPTH   3

class Fire {
	public: 
		int N;
		int dim;
		int size;
		float dt;

		float visc;
		float diff;

		float burnRate;
		float ignitionTemp;

		float r;
		float startVel;

		vector<float>d;
		vector<float>dOld;

		vector<float>u;
		vector<float>uOld;
		vector<float>v;
		vector<float>vOld;
		vector<float>w;
		vector<float>wOld;

		vector<float>t;
		vector<float>tOld;

		vector<float>fuel;
		vector<float>fuelOld;

		vector<float>su;
		vector<float>suOld;
		vector<float>sv;
		vector<float>svOld;
		vector<float>sw;
		vector<float>swOld;

		vector<float>st;
		vector<float>stOld;

		bool vorticity;

		// constructors
		Fire();
		Fire(int x);
		~Fire();

		int I(int x, int y, int z);
		void clear();
        void addSource(vector<float> &arr, vector<float> &arrOld);
		void boundary(int type, vector<float> &arr);
		void linearSolver(int type, vector<float> &arr, vector<float> &arrOld, float a, float c);
		void diffuse(int type, vector<float> &arr, vector<float> &arrOld, float dif);
		void backtrace(int type, vector<float> &arr, vector<float> &arrOld, vector<float> velX, vector<float> velY, vector<float> velZ);
		void cool(int type, vector<float> &arr, vector<float> &arrOld, vector<float> &temp, vector<float> &tempOld, vector<float> velX, vector<float> velY, vector<float> velZ);
		void project(vector<float>& p, vector<float>& div);
        void buoyancy(vector<float>& arr);
		vector<float> curl(int i, int j, int k);
		void vorticityConfinement(vector<float>& uVel, vector<float>& vVel, vector<float>& wVel);
		void burnFuel();
		void velocityStep();
		void densityStep();
		void tempStep();
};

#endif