#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include <stack>
#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>

#include "shoppingcart.hpp"
#include "../../database/database.hpp"

//------------------------------------------------

/* the createContent method creates the content specific for SHOPPINGCART */
void shoppingCart::createContent (void)
{
    string typedProduct;
    int typedAmount, typedPrice;

    //------------------------------------------------
    
    while (1)
    {
        cout << "Item: ";
        if (cin >> typedProduct && typedProduct == END)
            return;
        else if (isItemInShoppingCart(typedProduct))
            continue;

        //------------------------------------------------
        
        cout << "Amount: ";
        while (!(cin >> typedAmount) || typedAmount < 1)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
            cout << INVALID_AMOUNT << endl << "Amount: ";
        } // while
        
        //------------------------------------------------
        
        cout << "Price: ";
        while (!(cin >> typedPrice) || typedPrice < 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE);  // ignores multiple inputs
            cout << INVALID_PRICE << endl << "Price: ";
        } // while
    
        //------------------------------------------------
        
        item* newItem = new item(typedProduct, typedAmount, typedPrice);
        cart.push_back(newItem);
        cout << "PROGRAM: Item successfully created and saved to the shopping cart." << endl;
    } // while
} // CREATE CONTENT

//------------------------------------------------

/* the exportContent method exports content specific for SHOPPINGCART */
void shoppingCart::exportContent (ofstream & fileOut)
{
    fileOut << "NAME: "      << name      << endl
            << "TAG: "       << tag       << endl
            << "CATEGORY: "  << category  << endl
            << "TYPE: "      << type      << endl
            << "FORMATTED: " << formatted << endl
            << "CONTENT: "                << endl;
    
    for (auto & itemIt : cart)
    {
        fileOut << itemIt->getProduct() << " "
                << itemIt->getAmount()  << " "
                << itemIt->getPrice()   << endl;
    } // for
} // EXPORT CONTENT

//------------------------------------------------

/* the changeContent method changes the content of a SHOPPINGCART note */
bool shoppingCart::changeContent(void)
{
    string choice, typedProduct, typedCommand;
    int pos, typedValue;
    
    //------------------------------------------------
    
    cout << "<CONTENT EDITOR>==========<ON>" << endl
         << "Customize the note content"     << endl
         << "by /add, /delete and /change"   << endl
         << "commands."                      << endl
         << "To leave the content editor"    << endl
         << "write /finish."                 << endl
		 << DOUBLE_LINE                      << endl; // introduction
		 
    while (cin >> typedCommand)
    {
        if (typedCommand == ADD)
            createContent();
        else if (typedCommand == DELETE)
        {
            if (!(cin >> pos) ||
                pos < 1 ||
                pos > (int)cart.size())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
                cout << INVALID_POSITION << endl;
                isShoppingCartEmpty();
                continue;
            } // if
            
            cart.erase(cart.begin() + pos - 1); // deletes the item
        } // else if
        else if (typedCommand == CHANGE)
        {            
            if (!(cin >> pos) ||
                pos < 1 ||
                pos > (int)cart.size())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
                cout << INVALID_POSITION << endl;
                isShoppingCartEmpty();
                continue;
            } // if
            
            cout << "What do you want to change: [Product | Amount | Price]" << endl;
            while (!(cin >> choice) ||
                   (choice != "Product" &&
                    choice != "Amount" &&
                    choice != "Price"))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
                cout << INVALID_CHOICE << endl;
                continue;
            } // while

            if (choice == "Product" &&
                cout << "Write a new product: " &&
                cin >> typedProduct)
                cart.at(pos - 1)->setProduct(typedProduct);
            else if (choice == "Amount" &&
                     cout << "Write a new amount: ")
            {
                while (!(cin >> typedValue) ||
                       typedValue < 1)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
                    cout << INVALID_AMOUNT << endl << "Write a new amount: ";
                } // while
                cart.at(pos - 1)->setAmount(typedValue); 
            } // else if
            else if (choice == "Price" &&
                     cout << "Write a new price: ")
            {
                while (!(cin >> typedValue) ||
                       typedValue < 0)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
                    cout << INVALID_PRICE << endl << "Write a new price: ";
                } // while
                cart.at(pos - 1)->setPrice(typedValue); 
            } // if
        } // else if
        else if (typedCommand == FINISH)
        {
            cout << "<CONTENT EDITOR>=========<OFF>" << endl;
            break;
        } // else if
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), NEW_LINE); // ignores multiple inputs
            cout << "PROGRAM: Unknown command for CONTENT EDITOR." << endl;
            continue;
        } // else
        
        showContent(); // shows the note
    } // while
    
    return true;
} // CHANGE CONTENT

//------------------------------------------------

/* the importContent method imports content specific for SHOPPINGCART */
bool shoppingCart::importContent (ifstream & fileIn)
{
    string tmp,
		   fileProduct;
    unsigned fileAmount,
			 filePrice;
    
    //------------------------------------------------
    
    getline(fileIn, tmp);
    if (tmp.find("CONTENT: "))
    {
        cout << "PROGRAM: Format error while reading CONTENT." << endl;
        return false;
    } // if
        
    while (getline(fileIn, fileProduct, ' '))
    {
        if (getline(fileIn, tmp, ' '))
            fileAmount = atoi(tmp.c_str()); // char to integer
        else
        {
            cout << "PROGRAM: Format error while reading AMOUNT." << endl;
            return false;
        } // else

        if (getline(fileIn, tmp, NEW_LINE))
            filePrice = atoi(tmp.c_str()); // char to integer
        else
        {
            cout << "PROGRAM: Format error while reading PRICE." << endl;
            return false;
        } // else
        
        item* newItem = new item(fileProduct, fileAmount, filePrice);
        cart.push_back(newItem);    
    } // while
    
    return true;
} // IMPORT CONTENT

//------------------------------------------------

/* the showContent method shows the content of SHOPPINGCART note */
void shoppingCart::showContent (int editor)
{
    cout << SINGLE_LINE                << endl
         << "NAME: "      << name      << endl
         << "TAG: "       << tag       << endl
         << "CATEGORY: "  << category  << endl
         << "TYPE: "      << type      << endl
         << "FORMATTED: " << formatted << endl
         << "CONTENT: "                << endl;

    for (auto & itemIt : cart)
    {
        cout << "|| Product: " << itemIt->getProduct()
             << " | Amount: "  << itemIt->getAmount()
             << " | Price: "   << itemIt->getPrice() << endl;
    } // for
    
    calculateTotal();
    cout << "00============================" << endl
         << "Total: " << total               << endl
         << SINGLE_LINE                      << endl;
} // SHOW CONTENT

//------------------------------------------------

/* the doesItContain method tests whether it contains some string */
bool shoppingCart::doesItContain (const string & str)
{
    for (auto & itemIt : cart)
        if ((itemIt->getProduct()).find(str) != string::npos)
            return true;

    return false;
} // DOES IT CONTAIN

//------------------------------------------------

/* the checkPrice method tests whether the price meets the requirements */
bool shoppingCart::checkPrice (const string & str,
                               int value)
{
    calculateTotal();
    if ((str == "Less"  && (int)total < value) ||
        (str == "Equal" && (int)total == value) ||
        (str == "More"  && (int)total > value))
        return true;

    return false;
} // CHECK PRICE

//------------------------------------------------

/* the calculateTotal method calculates a final price according to all items in the shopping cart */
void shoppingCart::calculateTotal (void)
{
    total = 0;
    for (auto & itemIt : cart)
        total += itemIt->getAmount() * itemIt->getPrice();
} // CALCULATE TOTAL

//------------------------------------------------

/* the isItemInShoppingCart method tests whether the item is already in the shopping cart or not */
bool shoppingCart::isItemInShoppingCart (const string & item) const
{
    for (auto & itemIt : cart)
        if (itemIt->getProduct() == item)
        {
            cout << "PROGRAM: Shopping cart already contains the item \"" << item << "\"." << endl;
            return true;
        } // if

    return false;
} // IS ITEM IN SHOPPING CART

//------------------------------------------------

/* the isShoppingCartEmpty method tests whether the shopping cart is empty or not */
bool shoppingCart::isShoppingCartEmpty (void) const
{
    if (!cart.size())
    {
        cout << "PROGRAM: Shopping cart is empty." << endl;
        return true;
    } // if
    
    return false;
} // IS SHOPPING CART EMPTY
