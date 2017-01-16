/************************************************************************
* Author Name : bigzhao
* Email       : tandazhao@email.szu.edu.cn
* Filename    : EDAs.cpp
* Description : 使用 ACS 算法解决传统 TSP 问题，测试用例于 test_data.txt 中
为 oliver30.tsp
************************************************************************/
#include "ACS_TSP.h"

struct ant ants[POP];    // 全局蚁群
struct ant best_ant;     // 最优秀蚂蚁


/************************************************************************
* Function Name : fitness_evaluation
* Create Date : 2017/1/16
* Author/Corporation : bigzhao
**
Description : 计算总旅行距离的函数
*
* Param : *city: 需要计算的城市顺序
* distance: 距离矩阵
* return: 距离
**
************************************************************************/
double fitness_evaluation(int *city, double distance[][CITY_NUM])
{
	int i;
	double result = 0;

	for (i = 0; i < CITY_NUM; i++) {
		result += distance[city[i]][city[i + 1]];
	}
	return result;
}


/************************************************************************
* Function Name : find_the_best_ant
* Create Date : 2017/1/16
* Author/Corporation : bigzhao
**
Description : 用遍历的方法找蚁群里面最优秀的蚂蚁
*
* Param : *a: 蚁群
* return: 最优的下标
**
************************************************************************/
int find_the_best_ant(struct ant *a)
{
	int i, best_ant_index = 0;

	for (i = 1; i < POP; i++) {
		if (a[i].fitness < a[best_ant_index].fitness)
			best_ant_index = i;
	}
	return best_ant_index;
}


/************************************************************************
* Function Name : nearest_neighbor
* Create Date : 2017/1/16
* Author/Corporation : bigzhao
**
Description : 用遍历的方法找蚁群里面最优秀的蚂蚁
*
* Param : *a: 蚁群
* return: 最优的下标
**
************************************************************************/
double nearest_neighbor(double distance[][CITY_NUM])
{
	double d_nearest = 0.0; 
	int i = 0, j, shortest_index, index = 0, select_city[CITY_NUM];
	select_city[0] = 0;
	while (i < CITY_NUM - 1) {
		shortest_index = 1;
		if (index == 1)
			shortest_index++;
		while (1 == judge_exist_in_array(select_city, i + 1, shortest_index))
			shortest_index++;
		for (j = 1; j < CITY_NUM; j++) {
			if (index == j || 1 == judge_exist_in_array(select_city, i + 1, j))
				continue;
			if (distance[index][j] < distance[index][shortest_index])
				shortest_index = j;
		}
		//printf("%d %d\n", index, shortest_index);
		d_nearest += distance[index][shortest_index];
		index = shortest_index;
		select_city[i + 1] = shortest_index;
		//printf("%d\n", shortest_index);
		i++;
	}
	d_nearest += distance[select_city[CITY_NUM - 1]][0];
	//printf("near: %d %lf ", select_city[CITY_NUM - 1], d_nearest);
	return d_nearest;
}



/************************************************************************
* Function Name : pheromone_initialize
* Create Date : 2017/1/16
* Author/Corporation : bigzhao
**
Description : 信息素初始化
*
* Param : pheromone[][CITY_NUM]: 信息素矩阵
* tau0: 初始值，此值由贪婪算法求得
**
************************************************************************/
void pheromone_initialize(double pheromone[][CITY_NUM], double tau0)
{
	int i, j;

	for (i = 0; i < CITY_NUM; i++) {
		for (j = 0; j < CITY_NUM; j++) {
			if (i != j)
				pheromone[i][j] = tau0;
		}
	}
}

/************************************************************************
* Function Name : local_update_pheromone
* Create Date : 2017/1/16
* Author/Corporation : bigzhao
**
Description : 局部信息素更新
*
* Param : a: 需要更新的蚂蚁
* pheromone: 信息素矩阵
* tau0：初始值
**
************************************************************************/
void local_update_pheromone(struct ant a, double pheromone[][CITY_NUM], double tau0)
{
	int i;
	for (i = 0; i < CITY_NUM; i++) {
		pheromone[a.city[i]][a.city[i + 1]] *= (1 - ROU);
		pheromone[a.city[i]][a.city[i + 1]] += ROU * tau0;
		pheromone[a.city[i + 1]][a.city[i]] = pheromone[a.city[i]][a.city[i + 1]];
	}
}

/************************************************************************
* Function Name : global_update_pheromone
* Create Date : 2017/1/16
* Author/Corporation : bigzhao
**
Description : 全局信息素更新，全局更新只针对于最好的蚂蚁
*
* Param : best_ant: 需要更新的蚂蚁
* pheromone: 信息素矩阵
**
************************************************************************/
void global_update_pheromone(double pheromone[][CITY_NUM], struct ant best_ant)
{
	int i, j;
	int city_index = 0;
	for (i = 0; i < CITY_NUM; i++) {
		pheromone[best_ant.city[i]][best_ant.city[i + 1]] *= (1 - ALPHA);
		pheromone[best_ant.city[i]][best_ant.city[i + 1]] += 1.0 / best_ant.fitness;
		pheromone[best_ant.city[i + 1]][best_ant.city[i]] =
				pheromone[best_ant.city[i]][best_ant.city[i + 1]];
	}
}

/************************************************************************
* Function Name : ants_initialize
* Create Date : 2017/1/16
* Author/Corporation : bigzhao
**
Description : 蚂蚁初始化，确定每个蚂蚁的起始点
*
* Param : *a: 蚁群
**
************************************************************************/
void ants_initialize(struct ant *a)
{
	int i, j;
	double p;
	int random_index[CITY_NUM + 1];

	for (i = 0; i < CITY_NUM; i++)
		random_index[i] = i;
	std::random_shuffle(random_index, random_index + CITY_NUM);
	for (i = 0; i < POP; i++) {
		a[i].start = random_index[i];
	}

}

/************************************************************************
* Function Name : solution_construction
* Create Date : 2017/1/16
* Author/Corporation : bigzhao
**
Description : 蚂蚁的解的构造，按概率来操作，小于Q则挑选最大的，大于Q则
以赌轮盘的形式进行随机选择
*
* Param : *a: 蚁群
* distance: 距离矩阵
* pheromone：信息素矩阵
**
************************************************************************/
void solution_construction(struct ant *a, double distance[][CITY_NUM], double pheromone[][CITY_NUM])
{
	int i, j, k, max_index, city_not_be_selected[CITY_NUM], length_city_not_be_selected = 0;
	double q, sum = 0.0, probability, base;
	int remain_city[CITY_NUM] = {0};

	a->city[0] = a->start;
	a->city[CITY_NUM] = a->start;
	remain_city[a->start] = 1;


	for (i = 1; i < CITY_NUM; i++) {
		q = u01();
		max_index = 0;
		if (q < Q) {
			while (1 == remain_city[max_index])
				max_index++;
			for (j = 0; j < CITY_NUM; j++) {
				if (1 == remain_city[j])
					continue;
				if (pheromone[a->city[i - 1]][max_index] * pow(1 / distance[a->city[i - 1]][max_index], BETA)
					< pheromone[a->city[i - 1]][j] * pow(1 / distance[a->city[i - 1]][j], BETA))
					max_index = j;
			}
			a->city[i] = max_index;
			remain_city[max_index] = 1;
		}
		else {
			//否则就赌轮盘
			sum = 0.0;
			length_city_not_be_selected = 0;
			for (j = 0; j < CITY_NUM; j++) {
				if (1 == remain_city[j])
					continue;
				sum += pheromone[a->city[i - 1]][j] * pow(1.0 / distance[a->city[i - 1]][j], BETA);
				city_not_be_selected[length_city_not_be_selected++] = j;
			}

			probability = u01();

			base = 0.0;
			for (j = 0; j < length_city_not_be_selected; j++) {
				base += (pheromone[a->city[i - 1]][city_not_be_selected[j]]
					* pow(1.0 / distance[a->city[i - 1]][city_not_be_selected[j]], 2) / sum);
				if (probability < base) {
					a->city[i] = city_not_be_selected[j];
					remain_city[city_not_be_selected[j]] = 1;
					break;
				}
			}
		}
	}
	a->fitness = fitness_evaluation(a->city, distance);
}


/************************************************************************
* Function Name : ACS
* Create Date : 2017/1/16
* Author/Corporation : bigzhao
**
Description : 主算法，1）初始化信息素矩阵，2）初始化蚂蚁,3)对每个蚂蚁构造解，
并且对其进行局部信息素更新 4）选出最优蚂蚁与历史最优蚂蚁对比，选出最好的进行
全局更新 5)循环终止则跳出循环并输出，否则回到步骤 3）
*
* Param : distance: 距离矩阵
**
************************************************************************/
void ACS(double distance[][CITY_NUM])
{

	double pheromone[CITY_NUM][CITY_NUM] = { 0 };
	int iter, ant_index, best_ant_index, i, j;
	double tau0;
	tau0 = 1.0 / (CITY_NUM * nearest_neighbor(distance));

	pheromone_initialize(pheromone, tau0);
	ants_initialize(ants);
	for (iter = 0; iter < ITER; iter++) {
		for (ant_index = 0; ant_index < POP; ant_index++) {
			solution_construction(ants + ant_index, distance, pheromone);
			local_update_pheromone(ants[ant_index], pheromone, tau0);
		}
		best_ant_index = find_the_best_ant(ants);
		if (iter == 0 || ants[best_ant_index].fitness < best_ant.fitness)
			best_ant = ants[best_ant_index];
		global_update_pheromone(pheromone, best_ant);
	}

}



int main()
{

	double data[CITY_NUM][2] = { 0.0 };   
	int i, j;
	double distance[CITY_NUM][CITY_NUM] = { 0.0 };
	srand((unsigned)time(0));

	read_data("test_data.txt", data);
	calculate_distance_array(data, distance);

	for (j = 0; j < 1; j++) {
		ACS(distance);
		printf("The scheme is:");
		for (i = 0; i < CITY_NUM; i++)
			printf("%d -> ", best_ant.city[i]);
		printf("%d\n", best_ant.city[CITY_NUM]);
		printf("the total distance is %lf\n", best_ant.fitness);
	}

	return 0;
}
