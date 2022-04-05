import java.rmi.Naming;

public class Client {

    public static void loginAndStartAccount() {
        String password = MyUtility.insertPassword();

        try {
            InterfaceBank server = (InterfaceBank) Naming.lookup("//localhost:8080/bank_client_" + password);
            boolean stopCondition = false;

            while (!stopCondition) {
                switch (MyUtility.mainMenuResult()) {
                    case 1:
                        System.out.println("You current balance is: " + server.getBalance() + " $");
                        break;
                    case 2:
                        System.out.println("Type the amount to deposit:");
                        float amountToDeposit = MyUtility.getFloatInputFromConsole();
                        server.addMoney(amountToDeposit);
                        System.out.println("You successfully deposited " + amountToDeposit);
                        break;
                    case 3:
                        System.out.println("Type the amount to withdraw:");
                        float amountToWithdraw = MyUtility.getFloatInputFromConsole();
                        if (server.withdrawMoney(amountToWithdraw) == -1) {
                            System.out.println("You don't have all this money!");
                            break;
                        }
                        System.out.println("You successfully withdraw " + amountToWithdraw);
                    case 4:
                        stopCondition = true;
                        break;
                    default:
                        System.out.println("Error, operation not present in the menu!");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void handleLoginOrSignup() {
        try {
            InterfaceAuthBank serverAuth = (InterfaceAuthBank) Naming.lookup("//localhost:8080/bank_handle_login");
            final String password = MyUtility.askForNewPassword();
            final float startingBalance = MyUtility.askForStartingBalance();

            serverAuth.createAccount(startingBalance, password);
            System.out.println("New account created with starting balance: " + startingBalance + " and password: " + password);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        int firstMenuResult = MyUtility.firstMenuResult();
        switch(firstMenuResult) {
            case 1:
                handleLoginOrSignup();
                loginAndStartAccount();
                break;
            case 2:
                loginAndStartAccount();
                break;
            default:
                System.out.println("Error, operation not present in the menu!");
        }
    }
}
