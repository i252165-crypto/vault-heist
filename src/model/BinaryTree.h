#pragma once
#include "TreeNode.h"
#include "IntArray.cpp"
#include<iostream>

class BinaryTree{
    protected:
        TreeNode* root;

        void clearTree(TreeNode* node);

    public:
        BinaryTree();
        virtual ~BinaryTree();

        TreeNode* getRoot()const{ 
            return root;
        }

        void preOrderRecursive(TreeNode* node,IntArray& result);
        void preOrderIterative(IntArray& result);
        void inOrderRecursive(TreeNode* node,IntArray& result);
        void inOrderIterative(IntArray& result);
        void postOrderRecursive(TreeNode* node,IntArray& result);
        void postOrderIterative(IntArray& result);
        void levelOrder(IntArray& result);
        void levelOrder(IntArray& result);
        int getHeight(TreeNode* node);
        int getDiameter(TreeNode* node);
        bool isSameTree(TreeNode* p,TreeNode* q);
        bool isSymmetric(TreeNode* node);
        void mirror(TreeNode* node);
        TreeNode* lowestCommonAncestor(TreeNode* node,int val1,int val2);
        TreeNode* buildFromPreIn(int* preOrder,int* inOrder,int prelen,int inLen);
        TreeNode* buildFromPostIn(int* postOrder,int* inOrder,int postlen,int inLen);
};