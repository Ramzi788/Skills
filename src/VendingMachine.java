import java.util.EnumMap;
import java.util.HashMap;
import java.util.Map;

public interface VendingMachine {
    public void displayProducts();
    public void addProduct(String productName, int price, int quantity);
    public Product getProduct(String productName) throws ProductNotFoundException;
    public void insertCoin(Coin coin);
    public void dispenseProduct(String productName, int quantity);
    public Map<Coin, Integer> returnChange();

}
class VendingMachineImp implements VendingMachine{
    private int balance;
    private Map<String, Product> products;
    private Map<Coin, Integer> coinsInserted;
    public VendingMachineImp(){
        balance = 0;
        products = new HashMap<>();
        coinsInserted = new EnumMap<>(Coin.class);
        for (Coin coin : Coin.values()){
            coinsInserted.put(coin, 0);
        }
    }

    @Override
    public void displayProducts() {
        for (Map.Entry<String, Product> entry: products.entrySet()){
            System.out.println(entry.getKey() + "-PRICE: " + entry.getValue().getPrice() + "-REMAINING: " + entry.getValue().getQuantity());
        }
    }

    public void addProduct(String productName, int price, int quantity){
        products.computeIfAbsent(productName, k -> new Product(price, quantity));
    }

    @Override
    public Product getProduct(String productName) throws ProductNotFoundException {
        Product product = products.get(productName);
        if (product == null) {
            throw new ProductNotFoundException("NO PRODUCT AVAILABLE");
        } else if (balance < product.getPrice()) {
            throw new ProductNotFoundException("INSUFFICIENT FUNDS! PLEASE TRY AGAIN");
        }
        return product;
    }

    @Override
    public void insertCoin(Coin coin) {
        int count = coinsInserted.getOrDefault(coin, 0);
        coinsInserted.put(coin, count+1);
        balance += coin.getValue();
    }

    @Override
    public void dispenseProduct(String productName, int quantity) {
        Product product = products.get(productName);
        if (product == null){
            System.out.println("Product Not Found");
            return;
        }
        else if (quantity > product.getQuantity()){
            System.out.println("Too many requests for this product!");
            return;
        }
        else if (balance < product.getPrice()*quantity){
            System.out.println("Insufficient Funds");
            return;
        }

        product.setQuantity(product.getQuantity()-quantity);
        balance -= product.getPrice()*quantity;

    }

    @Override
    public Map<Coin, Integer> returnChange() {
        int changeAmount = this.balance;
        Map<Coin, Integer> change = new EnumMap<>(Coin.class);
        for (Coin coin: Coin.values()){
            int count = 0;
            while(changeAmount >= coin.getValue() && coinsInserted.get(coin) > 0){
                changeAmount -= coin.getValue();
                count ++;
                coinsInserted.put(coin, coinsInserted.get(coin) - 1);
            }
            if (count > 0){
                change.put(coin, count);
            }
        }
        balance = 0;
        return change;
    }

}

class Product{
    int price;
    int quantity;
    public Product(int price, int quantity){
        this.price = price;
        this.quantity = quantity;
    }
    public int getPrice() {
        return price;
    }
    public int getQuantity() {
        return quantity;
    }
    public void setQuantity(int quantity){
        this.quantity = quantity;
    }
}

enum Coin{
    five_cents(5), ten_cents(10), twenty_cents(20), fifty_cents(50), hundred_cents(100);
    private int value;
    Coin(int value){
        this.value = value;
    }
    public int getValue(){
        return this.value;
    }

}