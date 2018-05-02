#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#define LOW_INV 2000
#define GROW_SIZE 50


/*
 1.    Write the Product struct and ProductArray struct as defined
 in the document.  Make these structs use the typdef keyword to
 create data types.
 */

//structs
typedef struct {
    char id[5];
    char description[50];
    double price;
    int quantity;
} Product;

typedef struct {
    Product* array;
    int length;
    int quantity;
} ProductArray;

//prototypes
ProductArray make_product_array(int length);
void delete_product_array(ProductArray* prodArray);
void insert_product(ProductArray* prodArray, Product p);
ProductArray _read(void);
void print_prod(Product prod);
void print_all(ProductArray prodArray);
double calc_total(ProductArray prodArray);
void print_low(ProductArray prodArray);
void _write(ProductArray prodArray, double d);



/*
 6.    In the main function, declare a ProductArray and read the file.
 */

int main(int argc, const char * argv[]) {
    ProductArray prodArray = _read();
    print_all(prodArray);
    double total = calc_total(prodArray);
    printf("TOTAL: $%.2f\n\n\n", total);
    print_low(prodArray);
    _write(prodArray, total);
    delete_product_array(&prodArray);
    
    return 0;
}


/*
 2.    Write a function called make_product_array that creates and
 returns a ProductArray.  It should accept an integer for the initial
 length of the array and malloc memory for this array length.  Make
 sure to handle a malloc failure.  This function should return the
 PersonArray.
 */

ProductArray make_product_array(int length) {
    ProductArray prodArray;
    prodArray.array = (Product*) malloc(sizeof(Product) * length);
    if (prodArray.array == NULL) {
        perror("Error with malloc\n");
        exit(0);
    } else {
        prodArray.length = length;
        prodArray.quantity = 0;
    }
    return prodArray;
   
}



/*
 3.    Write a function called delete_product_array that accepts a
 pointer to a ProductArray.  The function should free the array memory
 and set length and count to zero.  It should return nothing.
 */

void delete_product_array(ProductArray* prodArray) {
    free(prodArray->array);
    prodArray->length = 0;
    prodArray->quantity = 0;
}


/*
 4.    Write a function called insert_product to insert a Product into a
 ProductArray.  It should accept a ProductArray pointer and a Product.
 It should check to see if the array is full (i.e. count == length).  If
 the array is full, it should reallocate, the array increasing the length
 by GROW_SIZE,  and check for realloc failure.  It should then copy the
 data from the Product to the ProductArray and add 1 to the count (suggest
 adding 1 instead of incrementing due to operator precedence issues).
 */
void insert_product(ProductArray* prodArray, Product p) {
    if (prodArray->quantity == prodArray->length) {
        prodArray->length += GROW_SIZE;
        prodArray->array = (Product*) realloc(prodArray->array, sizeof(Product) * GROW_SIZE);
    }
    else if (prodArray->length > prodArray->quantity) {
        strcpy(prodArray->array[prodArray->quantity].id, p.id);
        strcpy(prodArray->array[prodArray->quantity].description, p.description);
        prodArray->array[prodArray->quantity].price = p.price;
        prodArray->array[prodArray->quantity].quantity = p.quantity;
        prodArray->quantity = prodArray->quantity + 1;
    }
    
}

/*
 5.    Write a function called read to read a file of Product into a
 ProductArray.  The file name is Products.csv.  Open this file with notepad
 to see the data and decide on the best way to read it.  The function
 should accept the file name as an argument.  It should open the file for
 reading, call make_product_array to create an array of GROW_SIZE, and
 repetitively call insert_product to insert Products into the ProductArray
 until all Products in the file are read.  It should return a pointer to a
 ProductArray.  Remember to close the file.
 */

ProductArray _read() {
    ProductArray prodArray = make_product_array(GROW_SIZE);
    char* split;
    char buffer[100];
    FILE* in_file = fopen("Products.txt", "r");
    if (in_file == NULL) {
        perror("Error Opening File\n");
        exit(0);
    } else {
        while (fgets(buffer, 100, in_file) != NULL) {
            Product temp;
            buffer[strlen(buffer) - 1] = '\0';
            split = strtok(buffer, ",");
            strcpy(temp.id, split);
            split = strtok(NULL, ",");
            strcpy(temp.description, split);
            split = strtok(NULL, ",");
            temp.price = atof(split);
            split = strtok(NULL, ",");
            temp.quantity = atoi(split);
            insert_product(&prodArray, temp);
        }
    }
    fclose(in_file);
    return prodArray;
}
/*
 7.    Write a function called print to print a Product structís data to
 screen.  It should accept a Product as an argument.  It should return
 nothing.
 */

void print_prod(Product prod) {
    printf("ID:             %s\n", prod.id);
    printf("Description:    %s\n", prod.description);
    printf("Price:          %.2f\n", prod.price);
    printf("Quantity:       %d\n\n", prod.quantity);
}
/*
 8.    Write a function called print_all to print all of the ProductArrayís
 structs to screen.  This function should call the print function,
 repetitively, to print each Product in the ProductArray.  It should accept
 the ProductArray and return nothing.  Test this function by calling it
 from the main function.
 */

void print_all(ProductArray prodArray) {
    int i;
    for (i = 0; i < prodArray.quantity; i++) {
        print_prod(prodArray.array[i]);
    }
}

/*
 9.    Write a function called calc_total to calculate and return the total value
 of inventory in the Product array.  I suggest copying and pasting print_all
 and modifying the code to calculate the sum of price * quantity for all
 Products in the ProductArray.  It should accept the ProductArray and return the
 total value of inventory.  Test this function by calling it from the main
 function.
 */

double calc_total(ProductArray prodArray) {
    double total = 0;
    int i;
    for (i = 0; i < prodArray.quantity; i++) {
        total += (prodArray.array[i].price * prodArray.array[i].quantity);
    }
    
    return total;
}



/*
 10.    Write a function called Print_low to print all Products that are low in
 inventory to screen.  Low inventory is defined as less than 2000 units in stock.
 I suggest copying print_all and modifying the code to find all Products that are
 low in inventory.  It should accept the Product array and the number of elements
 in the array as arguments.  It should return nothing.  Use the constant LOW_INV
 for the test for low inventory.  Test this function by calling it from the main
 function.
 */

void print_low(ProductArray prodArray) {
    int i;
    for (i = 0; i < prodArray.quantity; i++) {
        if (prodArray.array[i].quantity < LOW_INV) {
            print_prod(prodArray.array[i]);
        }
    }
}



/*
 11.    Write a function called write to write the ProductArray to file as shown in
 the output file example below.  Note that Products with low inventory are marked
 with ì**** Low Inventoryî and that the total value of inventory is printed at the
 end of the file.  It should accept the ProductArray and the file name as
 arguments.  It should return nothing.  Test this function by calling it from the
 main function.
 */

void _write(ProductArray prodArray, double d) {
    FILE* out_file = fopen("InventoryResults.txt", "w");
    fprintf(out_file, "Inventory Report\n\n");
        for (int i = 0; i < prodArray.quantity; i++) {
            if (prodArray.array[i].quantity < LOW_INV) {
                fprintf(out_file, "ID:              %s\n", prodArray.array[i].id);
                fprintf(out_file, "Description:     %s\n", prodArray.array[i].description);
                fprintf(out_file, "Price:           %f\n", prodArray.array[i].price);
                fprintf(out_file, "Quantity:        %d %s\n\n", prodArray.array[i].quantity, "****Low Inventory");
            } else {
                fprintf(out_file, "ID:              %s\n", prodArray.array[i].id);
                fprintf(out_file, "Description:     %s\n", prodArray.array[i].description);
                fprintf(out_file, "Price:           %f\n", prodArray.array[i].price);
                fprintf(out_file, "Quantity:        %d\n\n", prodArray.array[i].quantity);
            }
        }
    fprintf(out_file, "TOTAL VALUE OF INVENTORY:    $%.2f\n", d);
    fclose(out_file);
}

