#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#include <unordered_map>
#include "HuffmanCode.h"

using namespace std;

void HuffmanCode::SetWeightTable(const vector<int>& data)
{
	for (int i = 0; i < data.size(); i++)
	{
		valToWeight[data[i]]++;
	}
}

void HuffmanCode::BuildTree()
{
	for (auto i : valToWeight)
	{
		remainNode.push(new HuffmanNode(i.first, i.second));
	}
	
	// 在这里跌倒三次了，呵呵
	int cnt = 0x3f3f3f3f;
	while (remainNode.size() > 1)
	{
		HuffmanNode* l = remainNode.top();
		remainNode.pop();
		HuffmanNode* r = remainNode.top();
		remainNode.pop();
		HuffmanNode* newRoot = new HuffmanNode(cnt++, l->w + r->w); // 非叶子节点不保存val
		newRoot->left = l;
		newRoot->right = r;
		remainNode.push(newRoot);
	}

	treeRoot = remainNode.top(); // 最终的树根
}

// dfs
void HuffmanCode::SearchCode(HuffmanNode* root, vector<char> code)
{
	
	if (root->left == nullptr && root->right == nullptr)
	{
		valToCode[root->val] = code; // val -> code
		return;
	}

	code.push_back('0');
	SearchCode(root->left, code);

	code[code.size() - 1] = '1';
	SearchCode(root->right, code);
}

void HuffmanCode::SetCodeTable()
{
	vector<char> empty;

	if(treeRoot != nullptr)
		SearchCode(treeRoot, empty);
}

vector<char> HuffmanCode::CharToBit(vector<char>& charSeq) // 最终的字节流
{
	// 从char高位到低位填入
	// 如char序列为 0 0 0 0 1 0 1 1
	// 则存在char里就是：最高位（第七位）为0，...，最低位（第零位）为1
	vector<char> bitSeq(1);
	int bit = 0;
	for (int i = 0; i < charSeq.size(); i++)
	{
		int code = charSeq[i] - '0';
		if (bit == 8)
		{
			bitSeq.push_back(0);
			bit = 0;
		}

		bitSeq[bitSeq.size() - 1] |= code << (7 - bit);
		bit++;
	}

	return bitSeq;
}

vector<char> HuffmanCode::SerializeMap()
{
	vector<uint32_t> data;
	uint32_t size = 0;
	for (auto i : valToWeight)
	{
		data.push_back(i.first);
		data.push_back(i.second);
		size++;
	}
	data.insert(data.begin(), size); // 头部为表大小/个数
	
	uint32_t *p = new uint32_t[data.size()];
	for (int i = 0; i < data.size(); i++)
		p[i] = data[i];

	char *p2 = reinterpret_cast<char*>(p);
	vector<char> result;
	for (int i = 0; i < data.size() * 4; i++)
		result.push_back(p2[i]);
	
	//for (int i = 0; i < result.size(); i++)
		//cout << (int)result[i] << " ";

	delete[] p;

	return result;
}

void HuffmanCode::DeserializeMap(vector<char> bitSeq, int size)
{
	uint32_t index = 4;
	for (int i = 0; i < size; i++)
	{
		char val[4];
		char weight[4];
		for (int j = 0; j < 4; j++)
			val[j] = bitSeq[index++];
		for (int j = 0; j < 4; j++)
			weight[j] = bitSeq[index++];
		int* pval = reinterpret_cast<int*>(val);
		int* pweight = reinterpret_cast<int*>(weight);
		valToWeight[*pval] = *pweight;

	}
}

vector<char> HuffmanCode::Encode(const vector<int>& data)
{
	// 除原本数据之外，将解码所需的valToWeight及所需字节数序列化至编码头部
	// headLength | head(hashmap) | bitLength | bitSequence
	vector<char> rawCode; // 编码 char
	vector<char> result; // 真正返回的bitstream
	uint32_t headLen = 0; // 序列化得到的头部长度

	int bitSize = 0;
	SetWeightTable(data);
	// map
	result = SerializeMap();
	// 只有一种val，文件: map 12 | bitsize 4 | bitseq
	if (valToWeight.size() == 1)
	{
		vector<char> res(data.size(), 0);
		res = CharToBit(res);
		int p = data.size();
		char* ptr = reinterpret_cast<char*>(&p);
		vector<char> bs(4);
		for (int i = 0; i < 4; i++)
			bs[i] = *(ptr+i);
		res.insert(res.begin(), bs.begin(), bs.end());
		res.insert(res.begin(), result.begin(), result.end());
		return res;
	}

	BuildTree();
	SetCodeTable();

	for (int i = 0; i < data.size(); i++)
	{
		vector<char> code = valToCode[data[i]]; // 编码的val
		for (int j = 0; j < code.size(); j++)
		{
			rawCode.push_back(code[j]);
			bitSize++;
		}
	}

	// bitsize
	char* ptr = reinterpret_cast<char*>(&bitSize);
	vector<char> tmp;
	for (int i = 0; i < 4; i++)
		tmp.push_back(ptr[i]);
	result.insert(result.end(), tmp.begin(), tmp.end());
	//cout << "bitsize: " << bitSize << endl;

	vector<char> bitData = CharToBit(rawCode);
	result.insert(result.end(), bitData.begin(), bitData.end());
	
	return result;
}

vector<char> HuffmanCode::BitToChar(const vector<char>& bitSeq, uint32_t bitCount)
{
	vector<char> charSeq(bitCount);
	int cnt = 0; // char vector下标
	int bitIdx = 0; // bit vector下标
	int bit = 0; // 当前char的位数

	while (cnt < bitCount)
	{
		if (bit == 8) // 当前char处理完
		{
			bitIdx++;
			bit = 0;
		}
		charSeq[cnt] = ((bitSeq[bitIdx]&(1<<(7-bit))) >> (7-bit)) + '0';
		cnt++;
		bit++;
	}

	return charSeq;
}

vector<int> HuffmanCode::Decode(const vector<char>& bitSeq) // 解码
{
	// 从char流提取map，得到bitsize后再解码
	// 前4个字节--map元素个数
	char p[4];
	for (int i = 0; i < 4; i++)
		p[i] = bitSeq[i];
	uint32_t *headLen = reinterpret_cast<uint32_t*>(p); // map size
	//cout << "map size: " << *headLen << endl;

	// 初始化
	treeRoot = nullptr;
	remainNode = priority_queue <HuffmanNode*, vector<HuffmanNode*>, cmp>();
	valToWeight.clear();
	valToCode.clear();
	
	// 得到权重表
	DeserializeMap(bitSeq, *headLen);

	// 提取bitsize
	uint32_t dataStart = *headLen * 8+ 4;
	for (int i = 0; i < 4; i++)
		p[i] = bitSeq[dataStart+i];
	uint32_t bitSize = *headLen;
	//cout << "start pos: " << dataStart << endl;
	//cout << "bit size: " << *headLen << endl;

	// 只有一种val，文件: map 12 | bitsize 4 | bitseq
	if (valToWeight.size() == 1)
	{
		vector<char> r(bitSeq.begin() + 16, bitSeq.end());
		vector<char> cs = BitToChar(r, bitSize);
		unordered_map<int, uint32_t>::iterator it = valToWeight.begin();
		vector<int> res(cs.size(), it->first);
		return res;
	}

	BuildTree();
	SetCodeTable();

	vector<char> dataSeq(bitSeq.begin()+dataStart+4, bitSeq.end());
	vector<char> charSeq = BitToChar(dataSeq, bitSize); // 先转char

	vector<int> result;
	HuffmanNode* ptr = treeRoot;
	
	for (int i = 0; i < bitSize; i++)
	{

		if (charSeq[i] == '0')
			ptr = ptr->left;
		else if (charSeq[i] == '1')
			ptr = ptr->right;

		if (ptr->left == nullptr && ptr->right == nullptr) // 得到一个val
		{
			result.push_back(ptr->val);
			ptr = treeRoot;
		}
	}

	return result;
}