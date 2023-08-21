/***
	编码
	input：将要编码的数据流（uchar）
	output：编码比特序列
	
	解码
	input：值频率表&比特序列
	output：uchar数据流
***/

#pragma once
#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;
// 我为啥用这么多vector

class HuffmanCode {
public:
	// 定义树节点
	struct HuffmanNode {
		HuffmanNode* left;
		HuffmanNode* right;
		uint32_t w;        // 当前节点权重
		int val; // 像素值，一个字节因为像素类型uchar即[0,255]
		HuffmanNode(int val, uint32_t w) : 
			val(val), w(w), left(nullptr), right(nullptr) {}
	};

	HuffmanCode(){ treeRoot = nullptr; }

	void BuildTree(); // 根据权重表建树

	void SetWeightTable(const vector<int>& data); // 权重表

	void SetCodeTable(); // 生成编码表，dfs遍历整颗树 得到叶节点val和path的编码

	void SearchCode(HuffmanNode* root, vector<char> code); // dfs

	vector<char> CharToBit(vector<char>& ); // 最终的bit流

	vector<char> BitToChar(const vector<char>& , uint32_t );

	vector<char> Encode(const vector<int>& data); // 编码入口

	vector<int> Decode(const vector<char>& bitSeq); // 解码

	vector<char> SerializeMap();

	void DeserializeMap(vector<char> bitSeq, int size);

private:
	unordered_map<int, uint32_t> valToWeight; // val对应的频率&权重
	unordered_map<int, vector<char> > valToCode; // val对应编码，这里的编码由'0''1'组成，并不是真正存入文件的bs，须uchar -> bit
	HuffmanNode* treeRoot; // 根节点
	struct cmp {
		bool operator()(HuffmanNode* a, HuffmanNode* b) {
			if (a->w == b->w)
				return a->val > b->val;
			return a->w > b->w;
		}
	};
	priority_queue <HuffmanNode*, vector<HuffmanNode*>, cmp> remainNode; // 用于建树的节点队列

};


