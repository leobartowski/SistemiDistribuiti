import java.util.Scanner;

public class MyUtility {

    // Methods
    public static int firstMenuResult() {
        Scanner scanner1 = new Scanner(System.in);
        int selection;
        System.out.println("---------------");
        System.out.println("Choose operation to perform:");
        System.out.println("1 - Create account");
        System.out.println("2 - Enter in your account");
        System.out.println("3 - Exit");
        System.out.print("Insert number of desired option: ");
        selection = scanner1.nextInt();
        return selection;
    }

    public static int mainMenuResult() {
        Scanner scanner1 = new Scanner(System.in);
        int selection;
        System.out.println("---------------");
        System.out.println("Choose operation to perform:");
        System.out.println("1 - Get Balance");
        System.out.println("2 - Deposit Money");
        System.out.println("3 - Withdraw Money");
        System.out.println("4 - Exit");
        System.out.print("Insert number of desired option: ");
        selection = scanner1.nextInt();
        return selection;
    }

    public static int optionToCreateNewAccountServer() {
        Scanner scanner1 = new Scanner(System.in);
        int selection;
        System.out.println("---------------");
        System.out.println("Create new account?");
        System.out.println("1 - Yes");
        System.out.println("2 - No");
        System.out.print("Insert number of desired option: ");
        selection = scanner1.nextInt();
        return selection;
    }

    public static float askForStartingBalance() {
        Scanner scanner1 = new Scanner(System.in);
        System.out.print("Type starting balance for your account: ");
        return scanner1.nextFloat();
    }

    public static String askForNewPassword() {
        Scanner scanner1 = new Scanner(System.in);
        System.out.print("Choose new password for your account: ");
        return scanner1.nextLine();
    }

    public static String insertPassword() {
        Scanner scanner1 = new Scanner(System.in);
        System.out.println("---------------");
        System.out.print("Insert Password: ");
        return scanner1.nextLine();
    }

    public static float getFloatInputFromConsole() {
        Scanner scanner1 = new Scanner(System.in);
        System.out.println("---------------");
        System.out.print("Insert amount:");
        return scanner1.nextFloat();
    }
}