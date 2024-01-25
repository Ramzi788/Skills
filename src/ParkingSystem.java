import java.lang.reflect.Array;
import java.util.*;
import java.util.stream.Collectors;

abstract class ParkingSpot{
    private String parkingSpotId;
    private boolean isSpotAvailable;
    private Vehicle vehicle;
    private ParkingSpotType parkingSpotType;

    public ParkingSpot(String parkingSpotId, ParkingSpotType parkingSpotType) {
        this.parkingSpotId = parkingSpotId;
        this.isSpotAvailable = true;
        this.vehicle = null;
        this.parkingSpotType = parkingSpotType;
    }
    public boolean isSpotFree(){
        return isSpotAvailable;
    }
    public ParkingSpotType getParkingSpotType(){
        return parkingSpotType;
    }
    public String getParkingSpotId(){
        return parkingSpotId;
    }
    public Vehicle getVehicleDetails() {
        return this.vehicle;
    }
    public void placeVehicleInSpot(Vehicle vehicle){
        if (!this.isSpotAvailable){
            System.out.println("NO SPOTS AVAILABLE");
        }
        this.vehicle = vehicle;
        this.isSpotAvailable = false;
    }
    public void vacateSpot(){
        this.vehicle = null;
        this.isSpotAvailable = true;
    }
}
class CarSpot extends ParkingSpot{
    CarSpot(String parkingSpotId){
        super(parkingSpotId, ParkingSpotType.COMPACT);
    }
}
class DisableSpot extends ParkingSpot{
    DisableSpot(String parkingSpotId){
        super(parkingSpotId, ParkingSpotType.DISABLED);
    }
}
class ElectricCarSpot extends ParkingSpot{
    ElectricCarSpot(String parkingSpotId){
        super(parkingSpotId, ParkingSpotType.ELECTRIC);
    }
}
class MotorcycleSpot extends ParkingSpot{
    MotorcycleSpot(String parkingSpotId){
        super(parkingSpotId, ParkingSpotType.MOTORCYCLE);
    }
}
enum ParkingSpotType{
    DISABLED, COMPACT, LARGE, MOTORCYCLE, ELECTRIC;
}
enum VehicleType{
    CAR, MOTORCYCLE, TRUCK, ELECTRIC;
}
class Vehicle{
   private String registerNumber;
   private VehicleType type;

    public Vehicle(String registerNumber, VehicleType type) {
        this.registerNumber = registerNumber;
        this.type = type;
    }

    public String getRegisterNumber() {
        return registerNumber;
    }
    public VehicleType getType() {
        return type;
    }

}
class Car extends Vehicle{
    Car(String registerNumber){
        super(registerNumber, VehicleType.CAR);
    }
}
class Truck extends Vehicle{
    Truck(String registerNumber){
        super(registerNumber, VehicleType.TRUCK);
    }
}
class Electric extends Vehicle{
    Electric(String registerNumber){
        super(registerNumber, VehicleType.ELECTRIC);
    }
}
class Motorcycle extends Vehicle{
    Motorcycle(String registerNumber){
        super(registerNumber, VehicleType.MOTORCYCLE);
    }
}

class ParkingFloor{
    private Map<ParkingSpotType, ArrayList<ParkingSpot>> parkingSpots = new HashMap<>();
    private String parkingFloorID;
    private DisplayBoard displayBoard;


    public ParkingFloor(String parkingFloorID) {
        this.parkingSpots.put(ParkingSpotType.COMPACT, new ArrayList<>());
        this.parkingSpots.put(ParkingSpotType.DISABLED, new ArrayList<>());
        this.parkingSpots.put(ParkingSpotType.ELECTRIC, new ArrayList<>());
        this.parkingSpots.put(ParkingSpotType.MOTORCYCLE, new ArrayList<>());
        this.parkingFloorID = parkingFloorID;
        this.displayBoard = new DisplayBoard();
    }
    public String getParkingFloorID() {
        return parkingFloorID;
    }
    public Map<ParkingSpotType, ArrayList<ParkingSpot>> getListOfParkingSpots() {
        return this.parkingSpots;
    }
    public ParkingSpot getAvailableSpot(Vehicle vehicle){
        ArrayList<ParkingSpot> parkingSpotsList = parkingSpots.get(this.getParkingSpotForVehicle(vehicle.getType()));
        for (ParkingSpot spot : parkingSpotsList){
            if (spot.isSpotFree()){
                return spot;
            }
        }
        return null;
    }
    private ParkingSpotType getParkingSpotForVehicle(VehicleType vehicleType){
        switch (vehicleType) {
            case CAR:
                return ParkingSpotType.COMPACT;
            case MOTORCYCLE:
                return ParkingSpotType.MOTORCYCLE;
            default:
                return ParkingSpotType.LARGE;
        }
    }
    public void showDisplayBoard(){
        for (ParkingSpotType type: parkingSpots.keySet()){
            long freeSpotsCount = parkingSpots.get(type).stream()
                    .filter(ParkingSpot::isSpotFree).count();
            displayBoard.displayMessage(freeSpotsCount, type);
        }
    }
    public ArrayList<ParkingSpot> getInUseSpotId(VehicleType vehicleType){
        ParkingSpotType spotType = getParkingSpotForVehicle(vehicleType);

        return new ArrayList<>(parkingSpots.get(spotType).stream()
                .filter(spot -> !spot.isSpotFree())
                .toList());
    }
    public boolean canPark(Vehicle vehicle){
        ParkingSpotType type = getParkingSpotForVehicle(vehicle.getType());
        return parkingSpots.get(type).stream().anyMatch(ParkingSpot::isSpotFree);
    }
}
class DisplayBoard{
    public void displayMessage(long freeSpots, ParkingSpotType parkingSpotType){
        System.out.println(parkingSpotType + " spots free: " + freeSpots);
    }
}

class ParkingLot{
    private static ParkingLot instance;
    private ArrayList<ParkingFloor> parkingFloors;
    private ArrayList<EntryPanel> entryPanels;
    private ArrayList<ExitPanel> exitPanels;

    private ParkingLot(){
        this.parkingFloors = new ArrayList<>();
        this.entryPanels = new ArrayList<>();
        this.exitPanels = new ArrayList<>();
    }
    public static synchronized ParkingLot getInstance(){
        if (instance == null) {
            instance = new ParkingLot();
        }
        return instance;
    }
    public ParkingSpot vacateParkingSpot(String parkingSpotID){
        for (ParkingFloor floor : parkingFloors){
            for (ArrayList<ParkingSpot> parkingSpots: floor.getListOfParkingSpots().values()){
                for(ParkingSpot parkingSpot: parkingSpots){
                    if (Objects.equals(parkingSpot.getParkingSpotId(), parkingSpotID)){
                        parkingSpot.vacateSpot();
                        return parkingSpot;
                    }
                }
            }
        }
        return null;
    }
    public ParkingSpot placeVehicleInSpot(Vehicle vehicle){
        for (ParkingFloor floor : parkingFloors){
            for (ArrayList<ParkingSpot> spots : floor.getListOfParkingSpots().values()){
                for (ParkingSpot parkingSpot: spots){
                    if (parkingSpot.isSpotFree()){
                        parkingSpot.placeVehicleInSpot(vehicle);
                        return parkingSpot;
                    }
                }
            }
        }
        return null;
    }

    public ArrayList<ParkingFloor> getParkingFloors() {
        return this.parkingFloors;
    }

    public ArrayList<EntryPanel> getEntryPanels() {
        return this.entryPanels;
    }

    public ArrayList<ExitPanel> getExitPanels() {
        return this.exitPanels;
    }
}

class EntryPanel{
    String entryPanelId;
    EntryPanel(String entryPanelId){
        this.entryPanelId = entryPanelId;
    }
    public String getEntryPanelId() {
        return entryPanelId;
    }
    public ParkingTicket getParkingTicket(Vehicle vehicle){
        ParkingFloor requestedFloor = null;
        for (ParkingFloor floor : ParkingLot.getInstance().getParkingFloors()){
            if (floor.canPark(vehicle)){
                requestedFloor = floor;
                break;
            }
        }
        assert requestedFloor != null;
        ParkingSpot spot = requestedFloor.getAvailableSpot(vehicle);
        ParkingTicket ticket = generateParkingTicket(
                vehicle,
                requestedFloor,
                spot.getParkingSpotId()
        );
        spot.placeVehicleInSpot(vehicle);
        return ticket;
    }
    private ParkingTicket generateParkingTicket(Vehicle vehicle, ParkingFloor parkingFloor, String parkingSpotId){
        return new ParkingTicket(vehicle.getType(), vehicle.getRegisterNumber(), parkingFloor.getParkingFloorID(), parkingSpotId);
    }

}
class ExitPanel{
    String exitPanelId;

    public ExitPanel(String exitPanelId) {
        this.exitPanelId = exitPanelId;
    }

    public String getExitPanelId() {
        return exitPanelId;
    }

    public void checkout(ParkingTicket parkingTicket){
        String parkingSpotId = parkingTicket.getParkingSpotID();
        double totalTime = parkingTicket.getEndTime().getTime() - parkingTicket.getStartTime().getTime();
        ParkingSpot vacatedSpot = ParkingLot.getInstance().vacateParkingSpot(parkingSpotId);
    }

}

class ParkingTicket{
    private VehicleType vehicleType;
    private String parkingTicketID;
    private String vehicleRegisterNumber;
    private String parkingSpotID;
    private String parkingFloorID;
    private Date startTime;
    private Date endTime;
    private double amount;

    public ParkingTicket(VehicleType vehicleType, String vehicleRegisterNumber, String parkingFloorID, String parkingSpotID){
        this.vehicleType = vehicleType;
        this.vehicleRegisterNumber = vehicleRegisterNumber;
        this.parkingFloorID = parkingFloorID;
        this.parkingSpotID = parkingSpotID;
        this.parkingTicketID = UUID.randomUUID().toString();
    }

    public Date getStartTime() {
        return startTime;
    }

    public void setStartTime(Date startTime) {
        this.startTime = startTime;
    }
    public void setStartTime(){
        this.startTime = new Date();
    }

    public Date getEndTime() {
        return endTime;
    }

    public void setEndTime(Date endTime) {
        this.endTime = endTime;
    }
    public void setEndTime(){
        this.endTime = new Date();
    }

    public double getAmount() {
        return amount;
    }

    public void setAmount(double amount) {
        this.amount = amount;
    }

    public VehicleType getVehicleType() {
        return vehicleType;
    }

    public String getVehicleRegisterNumber() {
        return vehicleRegisterNumber;
    }

    public String getParkingSpotID() {
        return parkingSpotID;
    }

    public String getParkingFloorID() {
        return parkingFloorID;
    }
}