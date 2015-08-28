#ifndef ASTNODE_H
#define ASTNODE_H

/**
 * @file ASTNode.h
 * @brief Defines a single node within an AST.
 *
 * @author Michael Albers
 */

#include <memory>
#include <ostream>
#include <string>
#include <vector>

class Token;

/**
 * Defines a node within an abstract syntax tree. Nodes are self-referential,
 * thus a each node is the root of a sub-tree (with one node being the
 * actual root of the tree).
 *
 * Each node owns each pointer to its children. In other words the caller
 * can freely 'new' objects for parameters to addChild.
 */
class ASTNode
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  ASTNode() = delete;

  ASTNode(const ASTNode &) = default;

  ASTNode(ASTNode &&) = default;

  ASTNode(const std::string &theNonTerminal);

  ASTNode(const Token &theToken);

  ~ASTNode() = default;

  ASTNode& operator=(const ASTNode &) = default;

  ASTNode& operator=(ASTNode &&) = default;

  void addChild(ASTNode *theChild);

  std::vector<std::shared_ptr<ASTNode>> getChildren() const noexcept;

  ASTNode* getParent() const noexcept;

  void traverse(std::ostream &theOS) const noexcept;

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Private
  // ************************************************************
  private:

  std::shared_ptr<std::string> myNonTerminal;

  std::shared_ptr<Token> myToken;

  ASTNode *myParent;

  std::vector<std::shared_ptr<ASTNode>> myChildren;
};

#endif
