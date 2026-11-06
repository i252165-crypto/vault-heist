#include "BinaryTree.h"

int Max(int a,int b){
    if(a>b)
        return a;
    else 
        return b;
}

BinaryTree::BinaryTree(){
    root=nullptr;
}

BinaryTree::~BinaryTree(){
    clearTree(root);
}

void BinaryTree::clearTree(TreeNode* node){
    if(node==nullptr){
        return;
    }

    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

void BinaryTree::preOrderRecursive(TreeNode* node,IntArray& result){
    if(node==nullptr){
        return;
    }

    result.push_back(node->value);
    preOrderRecursive(node->left,result);
    preOrderRecursive(node->right,result);
}

void BinaryTree::inOrderRecursive(TreeNode* node,IntArray& result){
    if(node==nullptr){
        return;
    }

    inOrderRecursive(node->left,result);
    result.push_back(node->value);
    inOrderRecursive(node->right,result);
}

void BinaryTree::postOrderRecursive(TreeNode* node,IntArray& result){
    if(node==nullptr){
        return;
    }

    postOrderRecursive(node->left,result);
    postOrderRecursive(node->right,result);
    result.push_back(node->value);
}

int BinaryTree::getHeight(TreeNode* node){
    if(node==nullptr){
        return 0;
    }

    int leftHeight=getHeight(node->left);
    int rightHeight=getHeight(node->right);

    return (Max(leftHeight,rightHeight)+1);
}

int BinaryTree::getDiameter(TreeNode* node){
    if(node==nullptr){
        return 0;
    }

    int leftHeight=getHeight(node->left);
    int rightHeight=getHeight(node->right);

    int leftDiameter=getDiameter(node->left);
    int rightDiameter=getDiameter(node->right);

    return Max(leftHeight+rightHeight+1,Max(leftDiameter,rightDiameter));
}

bool BinaryTree::isSameTree(TreeNode* p,TreeNode* q){
    if(p==nullptr && q==nullptr){
        return true;
    }

    if(p==nullptr || q==nullptr){
        return false;
    }

    return ((p->value == q->value) && isSameTree(p->left, q->left) && isSameTree(p->right, q->right));
}

void BinaryTree::mirror(TreeNode* node){
    if(node==nullptr){
        return;
    }

    TreeNode* temp=node->left;
    node->left=node->right;
    node->right=temp;

    mirror(node->left);
    mirror(node->right);
}

static bool isMirror(TreeNode* t1,TreeNode* t2){
    if(t1==nullptr && t2==nullptr){
        return true;
    }

    if(t1==nullptr || t2==nullptr){
        return false;
    }

    return (t1->value == t2->value) && isMirror(t1->left, t2->right) && isMirror(t1->right, t2->left);
}

bool BinaryTree::isSymmetric(TreeNode* node){
    if(node==nullptr){
        return true;
    }

    return isMirror(node->left,node->right);
}