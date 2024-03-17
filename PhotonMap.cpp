#include "PhotonMap.h"
#include <cmath>
#include <algorithm>

PhotonMap::PhotonMap() {
	maxPhotonNum = 10000;
	PhotonNum = 0;
	mPhoton = new Photon[10000 + 1];
	box_min = vec2(1000000.0f, 1000000.0f);
	box_max = vec2(-1000000.0f, -1000000.0f);
}
PhotonMap::PhotonMap(int max) {
	maxPhotonNum = max;
	PhotonNum = 0;
	mPhoton = new Photon[max + 1];
	box_min = vec2(FLT_MAX, FLT_MAX);
	box_max = vec2(-FLT_MAX, -FLT_MAX);
}
PhotonMap::~PhotonMap() {
	delete[] mPhoton;
}

void PhotonMap::cleanUp()
{
	PhotonNum = 0;
	box_min = vec2(FLT_MAX, FLT_MAX);
	box_max = vec2(-FLT_MAX, -FLT_MAX);
}

void PhotonMap::store(Photon photon) {
	if (PhotonNum >= maxPhotonNum) return;
	mPhoton[++PhotonNum] = photon;
	box_min = vec2(std::min(box_min.x, photon.Pos.x), std::min(box_min.y, photon.Pos.y));
	box_max = vec2(std::max(box_max.x, photon.Pos.x), std::max(box_max.y, photon.Pos.y));
}

// Ϊʲô���Ǵ����  start+num/2 �أ� ��Ȼstart+num/2 �ɱ�֤������������������ƽ��
// �������� �������������鱣��ģ��ں�һ��������� index �Ľڵ� �ӽڵ�Ϊ 2*index�� 2*index+1
// ��������ӽڵ����Ҷ�ӽڵ㣬��Ȼ���������ӽڵ㣬������ֻ�е���һ���ӽڵ㣬������Ҿ���ƽ�ֲ��У�����5���ڵ㣬��� 
// ����ƽ�� med�ڵ����3��  ��ߺ��ұ߶���2��������ʵ���ϲ��У���Ϊ���2���ڵ㣬ֻ��һ��father һ��child�ڵ㣬 father�ڵ�ֻ����һ������
// ͬ���ұ�Ҳfatherֻ��һ��child������  2*index�� 2*index+1�ļ��㣬�ұߵĻ�Խ�磬 2*3=6 > 5
// ���ʵ������ ��� 123 3���ڵ㣬 med��4�� �ұ�1 ���ڵ� 5 ����Ȼ������ƽ�������������������ʾ�Ķ���������Ҳ��ƽ���
// 
// ͬ���֪��������������ʾ�Ķ�����������������֮��ļ����ϵ����ô���Ծ���ƽ�֣�������������˷ѿռ䣩
int calMed(int start, int end) {
	int num = end - start + 1;
	int med;
 	int as = 1, b = 2;
 	while (as < num) {
 		as += b;
 		b *= 2;
 	}
	if (as == num)
		return start + num / 2;
 	b /= 2;
 	if (as - b / 2 < num) {
 		return start + as / 2;
 	}
 	else
 		return start + as / 2 - (as - b / 2 - num);
}


// ��֤med��߶�С��med��Ӧ��ֵ��med�ұ߶�����med��Ӧ��ֵ
void PhotonMap::MedianSplit(Photon* tempPhoton, int start, int end, int med, int axis) {
	int l = start, r = end;
	while (l < r) {
		double key = tempPhoton[r].Pos[axis];
		int i = l - 1, j = r;
		for (; ; ) {
			while (tempPhoton[++i].Pos[axis] < key);
			while (tempPhoton[--j].Pos[axis] > key && j > l);
			if (i >= j) break;
			std::swap(tempPhoton[i], tempPhoton[j]);
		}
		std::swap(tempPhoton[i], tempPhoton[r]);
		if (i >= med) r = i - 1;
		if (i <= med) l = i + 1;
	}
}

void PhotonMap::balance() {
	Photon* tempPhoton = new Photon[PhotonNum + 1];

	for (int i = 1; i <= PhotonNum; i++)
		tempPhoton[i] = mPhoton[i];

	balanceSegment(tempPhoton, 1, 1, PhotonNum);
	delete[] tempPhoton;
}

void PhotonMap::balanceSegment(Photon* tempPhoton, int index, int start, int end) {
	if (start == end) {
		mPhoton[index] = tempPhoton[start];
		mPhoton[index].axis = 100;
		return;
	}

	int med = calMed(start, end);

	int axis = 1;
	if (box_max.x - box_min.x > box_max.y - box_min.y) 
		axis = 0; 


	MedianSplit(tempPhoton, start, end, med, axis);
	mPhoton[index] = tempPhoton[med];
	mPhoton[index].axis = axis;

	if (start < med) {
		double tmp = box_max[axis];
		box_max[axis] = mPhoton[index].Pos[axis];
		balanceSegment(tempPhoton, index * 2, start, med - 1);
		box_max[axis] = tmp;
	}

	if (med < end) {
		double tmp = box_min[axis];
		box_min[axis] = mPhoton[index].Pos[axis];
		balanceSegment(tempPhoton, index * 2 + 1, med + 1, end);
		box_min[axis] = tmp;
	}
}

void PhotonMap::getNearestPhotons(Nearestphotons* np, int index) {
	if (index > PhotonNum) return;
	Photon *photon = &mPhoton[index];

	if (index * 2 <= PhotonNum) {
		double dist = np->Pos[photon->axis] - photon->Pos[photon->axis];
		if (dist < 0) {
			getNearestPhotons(np, index * 2);
			if (dist * dist < np->dist2[0]) getNearestPhotons(np, index * 2 + 1);
		}
		else {
			getNearestPhotons(np, index * 2 + 1);
			if (dist * dist < np->dist2[0]) getNearestPhotons(np, index * 2);
		}
	}

	float dist2 = pow(glm::length(photon->Pos-np->Pos),2.0f);
	if (dist2 > np->dist2[0]) return;

	if (np->found < np->max_photons) {
		np->found++;
		np->dist2[np->found] = dist2;
		np->photons[np->found] = photon;
	}
	else {
		// ʹ�õĶ������㷨
		// ������ �պ�N������һ�γ���N������ô��ʼ����
		if (np->got_heap == false) {
			for (int i = np->found >> 1; i >= 1; i--) {
				int par = i;
				Photon* tmp_photon = np->photons[i];
				float tmp_dist2 = np->dist2[i];
				while ((par << 1) <= np->found) {
					int j = par << 1;
					if (j + 1 <= np->found && np->dist2[j] < np->dist2[j + 1]) j++;
					if (tmp_dist2 >= np->dist2[j]) break;

					np->photons[par] = np->photons[j];
					np->dist2[par] = np->dist2[j];
					par = j;
				}
				np->photons[par] = tmp_photon;
				np->dist2[par] = tmp_dist2;
			}
			np->got_heap = true;
		}

		// ����ÿ�ζ�����󻯶ѶѶ�����Ȼ�������Ԫ�ز��뵽����λ��
		int par = 1;
		while ((par << 1) <= np->found) {
			int j = par << 1;
			if (j + 1 <= np->found && np->dist2[j] < np->dist2[j + 1]) j++;
			if (dist2 > np->dist2[j]) break;

			np->photons[par] = np->photons[j];
			np->dist2[par] = np->dist2[j];
			par = j;
		}
		np->photons[par] = photon;
		np->dist2[par] = dist2;

		np->dist2[0] = np->dist2[1];
	}
}

vec2 PhotonMap::getIrradiance(vec2 Pos, vec2 Norm, float max_dist, const int N) {
	vec2 ret(0.0,0.0);
	Nearestphotons np;
	np.Pos = Pos;
	np.max_photons = N;
	np.dist2 = new float[N + 1];
	np.photons = new Photon*[N + 1];
	np.dist2[0] = max_dist * max_dist;
	getNearestPhotons(&np, 1);
	if (np.found <= 8) return ret;
	//����������ǹ��Ӿ����ҵ���N�����ӵ������룬��
	//max_dist���������õ�������
	float maxDist = sqrtf(np.dist2[0]);
	const float k = 1.1f;
	for (int i = 1; i <= np.found; i++) {
		vec2 dir = np.photons[i]->Dir;
		float wht = 1.0f - (np.Pos - np.photons[i]->Pos).length() / (k * maxDist);
		if (dot(Norm,dir) < 0) ret = ret +  wht * np.photons[i]->power;  //
	}
	ret = ret * (1 / (1000000.0f * np.dist2[0] * (1 - 2.0f / (3 * k))  ));  //
	return ret;
}



















