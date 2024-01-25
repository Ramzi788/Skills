import java.lang.reflect.Array;
import java.util.*;

public class Hotel {
    private static Hotel instance;
    private ArrayList<Floors> floors;
    private Map<String, Reservation> reservations;

    private Hotel(){
        this.floors = new ArrayList<>();
        this.reservations = new HashMap<>();
    }
    public static synchronized Hotel getInstance(){ // synchronized for thread safety
        if (instance == null){
            instance = new Hotel();
        }
        return instance;
    }
    public Room reserveRoom(Guest guest, String roomID) throws Exception {
        for (Floors floor:floors){
            for(ArrayList<Room> rooms: floor.getListOfRooms().values()){
                for (Room room : rooms){
                    if (Objects.equals(room.getRoomId(), roomID)){
                        Hotel.getInstance().getReservation(guest);
                        return room;
                    }
                }
            }
        }
        return null;
    }
    public Reservation getReservation(Guest guest) throws Exception {
        Floors requestedFloor = null;
        for (Floors floor: Hotel.getInstance().getFloors()){
            if (floor.canReserve(guest)){
                requestedFloor = floor;
                break;
            }
        }
        assert requestedFloor != null;
        Room room = requestedFloor.getAvailableRooms(guest);
        Reservation reservation = generateReservation(
                guest,
                requestedFloor,
                room.getRoomId()
        );
        reservations.put(room.getRoomId(), reservation);
        room.reserveRoomForGuest(guest);
        return reservation;
    }
    private Reservation generateReservation(Guest guest, Floors floor, String roomID){
        return new Reservation(guest.getTypeOfGuest(), roomID, floor.getHotelFloorID());
    }

    public Room vacateRoom(String roomID){
        for (Floors floor: floors){
            for (ArrayList<Room> rooms: floor.getListOfRooms().values()){
                for (Room room : rooms){
                    if (Objects.equals(room.getRoomId(), roomID)){
                        room.vacateRoom();
                        break;
                    }
                }
            }
        }
        return null;
    }

    public ArrayList<Floors> getFloors() {
        return floors;
    }
    public ArrayList<Reservation> getReservations(){
        ArrayList<Reservation> listOfReservations = new ArrayList<>();
        for (Map.Entry<String, Reservation> reservationEntry: reservations.entrySet()){
            listOfReservations.add(reservationEntry.getValue());
        }
        return listOfReservations;
    }
    public ArrayList<Room> getFreeRooms(){
        ArrayList<Room> listOfFreeRooms = new ArrayList<>();
        for (Floors floor: floors ){
            listOfFreeRooms.addAll(floor.getAvailableRooms());
        }
        return listOfFreeRooms;
    }

}

class Reservation{
    private GuestType guestType;
    private String reservationId;
    private String roomID;
    private String hotelFloorID;
    private double amount;

    public Reservation(GuestType guestType,String roomID, String hotelFloorID){
        this.guestType = guestType;
        this.reservationId = UUID.randomUUID().toString();
        this.roomID = roomID;
        this.hotelFloorID = hotelFloorID;
    }

    public GuestType getGuestType() {
        return guestType;
    }

    public void setGuestType(GuestType guestType) {
        this.guestType = guestType;
    }

    public String getReservationId() {
        return reservationId;
    }

    public void setReservationId(String reservationId) {
        this.reservationId = reservationId;
    }


    public String getRoomID() {
        return roomID;
    }

    public void setRoomID(String roomID) {
        this.roomID = roomID;
    }

    public String getHotelFloorID() {
        return hotelFloorID;
    }

    public void setParkingFloorID(String parkingFloorID) {
        this.hotelFloorID = parkingFloorID;
    }

    public double getAmount() {
        return amount;
    }

    public void setAmount(double amount) {
        this.amount = amount;
    }
    public String toString(){
        return "Room{" +
                "roomId='" + roomID + '\'' +
                ", guest=" + ( guestType!= null ? guestType.name() : "none") +
                ", roomsType=" + reservationId +
                '}';
    }
}
class Guest{
    String name;
    GuestType typeOfGuest;
    int paymentAmount;
    Reservation reservation;

    public Guest(String name, GuestType typeOfGuest, Reservation reservation, int paymentAmount){
        this.name = name;
        this.typeOfGuest = typeOfGuest;
        this.reservation = reservation;
        this.paymentAmount = paymentAmount;
    }
    public GuestType getTypeOfGuest() {
        return typeOfGuest;
    }
    public String getName() {
        return name;
    }
    public Reservation getReservation() {
        return reservation;
    }

    public int getPaymentAmount() {
        return paymentAmount;
    }
}
enum GuestType{
    SINGLE, COUPLE, FAMILYOFTHREE;
}
class Room{
    private String roomId;
    private boolean isRoomAvailable;
    private Guest guest;
    private RoomsType roomsType;

    public Room(String roomId, RoomsType roomsType){
        this.roomId = roomId;
        this.isRoomAvailable = true;
        this.guest = null;
        this.roomsType = roomsType;
    }
    public String getRoomId() {
        return roomId;
    }
    public boolean isRoomFree() {
        return isRoomAvailable;
    }
    public Guest getGuestDetails() {
        return guest;
    }
    public RoomsType getRoomsType() {
        return roomsType;
    }
    public void reserveRoomForGuest(Guest guest){
        if (!this.isRoomAvailable)
            System.out.println("ROOM NOT AVAILABLE");
        this.guest = guest;
        this.isRoomAvailable = false;
    }
    public void vacateRoom(){
        this.guest = null;
        this.isRoomAvailable = true;
    }
    @Override
    public String toString() {
        return "Room{" +
                "roomId='" + roomId + '\'' +
                ", isRoomAvailable=" + isRoomAvailable +
                ", guest=" + (guest != null ? guest.getName() : "none") +
                ", roomsType=" + roomsType +
                '}';
    }


}
enum RoomsType{
    SINGLE(50), DOUBLE(75), SUITE(125);
    int price;
    RoomsType(int price){
        this.price = price;
    }
}
class Floors{
    Map<RoomsType, ArrayList<Room>> rooms;  //Number of rooms for each type
    String hotelFloorID;

    Floors(String hotelFloorID){
        this.hotelFloorID = hotelFloorID;
        rooms = new EnumMap<>(RoomsType.class);
        for (RoomsType type: RoomsType.values()){
            this.rooms.put(type, new ArrayList<>());
        }
    }

    public Map<RoomsType, ArrayList<Room>> getListOfRooms(){
        return rooms;
    }
    public String getHotelFloorID() {
        return hotelFloorID;
    }
    public Room getAvailableRooms(Guest guest){
        ArrayList<Room> availableRooms = rooms.get(getRoomForGuest(guest.getTypeOfGuest()));
        for (Room room: availableRooms){
            if (room.isRoomFree()){
                return room;
            }
        }
        return null;
    }
    public ArrayList<Room> getAvailableRooms(){
        ArrayList<Room> availableRooms = new ArrayList<>();
        for (Map.Entry<RoomsType, ArrayList<Room>> room: rooms.entrySet()){
            for (Room room1: room.getValue()){
                if (room1.isRoomFree()){
                    availableRooms.add(room1);
                }
            }
        }
        return availableRooms;
    }
    public ArrayList<Room> getReservedRooms(GuestType guestType){
        RoomsType roomType = getRoomForGuest(guestType);
        return new ArrayList<>(rooms.get(roomType).stream().filter(room -> !room.isRoomFree()).toList());
    }
    public boolean canReserve(Guest guest) throws Exception{
        RoomsType type = getRoomForGuest(guest.getTypeOfGuest());
        if (guest.paymentAmount < type.price){
            throw new Exception("INSUFFICIENT FUNDS");
        }
        return rooms.get(type).stream().anyMatch(Room::isRoomFree);
    }
    private RoomsType getRoomForGuest(GuestType guestType){
        switch (guestType){
            case SINGLE:
                return RoomsType.SINGLE;
            case FAMILYOFTHREE:
                return RoomsType.SUITE;
            default:
                return RoomsType.DOUBLE;

        }
    }

}
