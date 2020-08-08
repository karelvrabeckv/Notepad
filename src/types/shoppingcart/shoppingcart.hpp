#pragma once

#include "../../constants.hpp"
#include "../../database/database.hpp"

//////////////////////////////////////////////////
// ITEM
//////////////////////////////////////////////////

/** ITEM represents a PRODUCT stored in SHOPPING CART */
struct item
{
        
	private:
		
		string product;
		unsigned amount,
				 price;

	public:
				 
		item (const string & typedProduct,
			  unsigned typedAmount,
			  unsigned typedPrice) : product (typedProduct),
									 amount  (typedAmount),
									 price   (typedPrice) {}

		/** setters of item */
		void setAmount  (unsigned typedAmount)           {amount  = typedAmount;}
		void setPrice   (unsigned typedPrice)            {price   = typedPrice;}
		void setProduct (const    string & typedProduct) {product = typedProduct;}

		/** getters of item */
		unsigned getAmount  (void) {return amount;}
		unsigned getPrice   (void) {return price;}
		string   getProduct (void) {return product;}
  
}; // ITEM

//////////////////////////////////////////////////
// SHOPPING CART
//////////////////////////////////////////////////

/** SHOPPING CART is a NOTE TYPE and CLASS DESCENDANT of NOTE */
class shoppingCart : public note
{
    private:

        vector <item*> cart;
        unsigned total;
        
    public:
        
        shoppingCart (const string & typedName,
                      const string & typedTag,
                      const string & typedCategory,
                      const string & typedType,
                      const string & typedFormatted)
        {
            name = typedName;
            tag = typedTag;
            category = typedCategory;
            type = typedType;
            formatted = typedFormatted;
        } // CONSTRUCTOR
        
        virtual ~shoppingCart (void)
        {
            for (auto & itemIt : cart)
                delete itemIt;
        } // DESTRUCTOR
        
		/** the createContent method creates the content specific for SHOPPINGCART */
        virtual void createContent (void);
		/** the exportContent method exports content specific for SHOPPINGCART */
        virtual void exportContent (ofstream & fileOut);
		/** the changeContent method changes the content of a SHOPPINGCART note */
        virtual bool changeContent (void);
		/** the importContent method imports content specific for SHOPPINGCART */
        virtual bool importContent (ifstream & fileIn);
		/** the showContent method shows the content of SHOPPINGCART note */
        virtual void showContent   (int editor = EDITOR_OFF);
        
		/** the doesItContain method tests whether it contains some string */
        virtual bool doesItContain (const string & str);
		/** the checkPrice method tests whether the price meets the requirements */
        virtual bool checkPrice    (const string & str,
                                    int value);
        
		/** the calculateTotal method calculates a final price according to all items in the shopping cart */
        void calculateTotal       (void);
		/** the isItemInShoppingCart method tests whether the item is already in the shopping cart or not */
        bool isItemInShoppingCart (const string & item) const;
		/** the isShoppingCartEmpty method tests whether the shopping cart is empty or not */
        bool isShoppingCartEmpty  (void) const;
        
}; // SHOPPING CART
