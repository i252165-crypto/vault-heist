#pragma once
class TreeNode{
    public:
        int id;
        int value;
        TreeNode* left;
        TreeNode* right;
        TreeNode* parent;
        int height;
        int size;

        inline static int s_next_ID=1;

        TreeNode(int val){
            id = s_next_ID++;
            value=val;
            left=nullptr;
            right=nullptr;
            parent=nullptr;
            height=1;
            size=1;
        }

        ~TreeNode(){}

        TreeNode(const TreeNode&)=delete;

        TreeNode& operator=(const TreeNode&)=delete;

        static void resetIds(){
            s_next_ID=1;
        }
};