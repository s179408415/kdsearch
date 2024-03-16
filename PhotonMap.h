#pragma once
#ifndef __PHOTONMAP_H__
#define __PHOTONMAP_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QtWidgets/qgraphicsitem.h>
using namespace glm;

struct Photon {
	vec2 Pos; //λ��
	vec2 Dir; //���䷽��
	vec2 power; //������ͨ������ɫֵ��ʾ
	QGraphicsItem* item;
	int axis;
};

struct Nearestphotons {
	vec2 Pos;
	int max_photons, found;
	bool got_heap;
	float* dist2;
	Photon** photons;
	Nearestphotons() {
		max_photons = found = 0;
		got_heap = false;
		dist2 = NULL;
		photons = NULL;
	}
	~Nearestphotons() {
		delete[] dist2;
		delete[] photons;
	}
};

class PhotonMap {
public:
	PhotonMap();
	PhotonMap(int max);
	~PhotonMap();
	int maxPhotonNum;
	int PhotonNum;//��������
	Photon *mPhoton;
	void store(Photon pn);
	void cleanUp();
	void MedianSplit(Photon* porg, int start, int end, int med, int axis);
	void balance();
	void PhotonMap::balanceSegment(Photon*, int, int, int);
	bool getPhoton(Photon &pn, int index) {
		if (index > maxPhotonNum) return false;
		else {
			pn = mPhoton[index];
			return true;
		}
	}
	void getNearestPhotons(Nearestphotons* np, int index);
	float getPhotonPosAxis(int index, int axis) {
		return mPhoton[index].Pos[axis];
	}
	vec2 getIrradiance(vec2 Pos, vec2 Norm, float max_dist, const int N);
	vec2 box_min, box_max;

};







#endif



