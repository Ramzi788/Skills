import org.w3c.dom.events.Event;

import java.util.HashMap;
import java.util.Map;

class UndergroundSystem {
    private final String DELIMETER = ",";
    Map<Integer, Passenger> arrivals;
    Map<String, Average> average;
    public UndergroundSystem() {
        arrivals = new HashMap<>();
        average = new HashMap<>();
    }

    public void checkIn(int id, String stationName, int t) {
        arrivals.put(id, new Passenger(id, stationName, t));
    }

    public void checkOut(int id, String stationName, int t) {
        Passenger arrived = arrivals.get(id);
        arrivals.remove(id);

        int diff = t - arrived.t;
        String initialStation = arrived.stationName;
        String key = initialStation + DELIMETER + stationName;

        Average averageTime = average.containsKey(key) ? average.get(key) : new Average();
        averageTime.updateAverage(diff);

        average.put(key, averageTime);
    }

    public double getAverageTime(String startStation, String endStation) {
        String key = startStation + DELIMETER + endStation;
        return average.get(key).getAverage();
    }
    class Passenger{
        int id;
        String stationName;
        int t;
        Passenger(int id, String stationName, int t){
            this.id = id;
            this.stationName = stationName;
            this.t = t;
        }
    }

    class Average{
        double total = 0.0;
        int count = 0;

        public void updateAverage(int diff){
            count++;
            total+= diff;
        }

        public double getAverage(){
            return total/count;
        }
    }

}