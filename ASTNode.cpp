/**
 * @file ASTNode.cpp
 * @brief Implementation of ASTNode class
 *
 * @author Michael Albers
 */

#include "ASTNode.h"
#include "Token.h"

//*****************
// ASTNode::ASTNode
//*****************
ASTNode::ASTNode(const std::string &theNonTerminal) :
  myNonTerminal{new std::string{theNonTerminal}},
  myParent(nullptr)
{
}

//*****************
// ASTNode::ASTNode
//*****************
ASTNode::ASTNode(const Token &theToken) :
  myToken{new Token{theToken}}
{
}

//******************
// ASTNode::addChild
//******************
void ASTNode::addChild(ASTNode *theChild)
{
  theChild->myParent = this;
  myChildren.push_back(std::shared_ptr<ASTNode>{theChild});
}

//*********************
// ASTNode::getChildren
//*********************
std::vector<std::shared_ptr<ASTNode>> ASTNode::getChildren() const noexcept
{
  return myChildren;
}

//*******************
// ASTNode::getParent
//*******************
ASTNode* ASTNode::getParent() const noexcept
{
  return myParent;
}

//******************
// ASTNode::traverse
//******************
void ASTNode::traverse(std::ostream &theOS) const noexcept
{
  if (myChildren.empty())
  {
    if (myToken.get())
    {
      theOS << myToken->getTokenString();
    }
    else
    {
      theOS << *myNonTerminal;
    }
    theOS << " ";
  }
  else
  {
    for (auto child : myChildren)
    {
      child->traverse(theOS);
    }
  }
}
