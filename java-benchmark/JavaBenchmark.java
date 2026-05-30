import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;

public class JavaBenchmark {

    private static final int RECORDS = 10000;
    private static final String OUTPUT_FILE = "java_results.csv";

    public static void main(String[] args) {
        System.out.println("==============================================");
        System.out.println(" BENCHMARK JAVA - RED SOCIAL ACADEMICA");
        System.out.println("==============================================");
        System.out.println("Registros utilizados: " + RECORDS);
        System.out.println();

        StudentJava[] students = generateStudents(RECORDS);

        BenchmarkResult hashInsert = benchmarkHashMapInsert(students);
        BenchmarkResult hashSearch = benchmarkHashMapSearch(students);
        BenchmarkResult treeInsert = benchmarkTreeMapInsert(students);
        BenchmarkResult treeTraversal = benchmarkTreeMapTraversal(students);

        writeResults(hashInsert, hashSearch, treeInsert, treeTraversal);

        System.out.println();
        System.out.println("Archivo generado correctamente: " + OUTPUT_FILE);
        System.out.println("Contenido esperado:");
        System.out.println("language,operation,structure,records,time_ms");
        System.out.println("Java,insert,HashMap,10000,...");
        System.out.println("Java,search,HashMap,10000,...");
        System.out.println("Java,insert,TreeMap,10000,...");
        System.out.println("Java,traversal,TreeMap,10000,...");
    }

    private static StudentJava[] generateStudents(int total) {
        StudentJava[] students = new StudentJava[total];

        for (int i = 0; i < total; i++) {
            int id = 100000 + i + 1;
            String name = "Benchmark Student " + (i + 1);
            String career = "Ingenieria en Sistemas";
            int semester = (i % 10) + 1;
            double gpa = 60.0 + ((i % 4000) / 100.0);
            int skillScore = (i * 37) % 100000;

            students[i] = new StudentJava(
                    id,
                    name,
                    career,
                    semester,
                    gpa,
                    skillScore
            );
        }

        return students;
    }

    private static BenchmarkResult benchmarkHashMapInsert(StudentJava[] students) {
        HashMap<Integer, StudentJava> hashMap = new HashMap<>();

        long start = System.nanoTime();

        for (StudentJava student : students) {
            hashMap.put(student.getStudentId(), student);
        }

        long end = System.nanoTime();

        double timeMs = toMilliseconds(start, end);

        System.out.println("HashMap insert: " + format(timeMs) + " ms");

        return new BenchmarkResult(
                "Java",
                "insert",
                "HashMap",
                students.length,
                timeMs
        );
    }

    private static BenchmarkResult benchmarkHashMapSearch(StudentJava[] students) {
        HashMap<Integer, StudentJava> hashMap = new HashMap<>();

        for (StudentJava student : students) {
            hashMap.put(student.getStudentId(), student);
        }

        int found = 0;

        long start = System.nanoTime();

        for (StudentJava student : students) {
            StudentJava result = hashMap.get(student.getStudentId());

            if (result != null) {
                found++;
            }
        }

        long end = System.nanoTime();

        double timeMs = toMilliseconds(start, end);

        System.out.println("HashMap search: " + format(timeMs) + " ms");
        System.out.println("HashMap encontrados: " + found);

        return new BenchmarkResult(
                "Java",
                "search",
                "HashMap",
                students.length,
                timeMs
        );
    }

    private static BenchmarkResult benchmarkTreeMapInsert(StudentJava[] students) {
        TreeMap<Long, StudentJava> treeMap = new TreeMap<>();

        long start = System.nanoTime();

        for (StudentJava student : students) {
            long key = createTreeKey(student);
            treeMap.put(key, student);
        }

        long end = System.nanoTime();

        double timeMs = toMilliseconds(start, end);

        System.out.println("TreeMap insert: " + format(timeMs) + " ms");

        return new BenchmarkResult(
                "Java",
                "insert",
                "TreeMap",
                students.length,
                timeMs
        );
    }

    private static BenchmarkResult benchmarkTreeMapTraversal(StudentJava[] students) {
        TreeMap<Long, StudentJava> treeMap = new TreeMap<>();

        for (StudentJava student : students) {
            long key = createTreeKey(student);
            treeMap.put(key, student);
        }

        int visited = 0;

        long start = System.nanoTime();

        for (Map.Entry<Long, StudentJava> entry : treeMap.entrySet()) {
            StudentJava student = entry.getValue();

            if (student != null) {
                visited++;
            }
        }

        long end = System.nanoTime();

        double timeMs = toMilliseconds(start, end);

        System.out.println("TreeMap traversal: " + format(timeMs) + " ms");
        System.out.println("TreeMap visitados: " + visited);

        return new BenchmarkResult(
                "Java",
                "traversal",
                "TreeMap",
                students.length,
                timeMs
        );
    }

    private static long createTreeKey(StudentJava student) {
        /*
            TreeMap ordena por clave.
            Para simular el orden por skill_score y evitar problemas con scores repetidos,
            se crea una clave compuesta:
            skill_score primero + student_id como desempate.

            Esto permite comparar TreeMap con el AVL de C++ usado para ranking academico.
        */
        return ((long) student.getSkillScore() * 1_000_000L) + student.getStudentId();
    }

    private static double toMilliseconds(long start, long end) {
        return (end - start) / 1_000_000.0;
    }

    private static String format(double value) {
        return String.format("%.4f", value);
    }

    private static void writeResults(
            BenchmarkResult hashInsert,
            BenchmarkResult hashSearch,
            BenchmarkResult treeInsert,
            BenchmarkResult treeTraversal
    ) {
        try (PrintWriter writer = new PrintWriter(new FileWriter(OUTPUT_FILE))) {
            writer.println("language,operation,structure,records,time_ms");
            writer.println(hashInsert.toCsvLine());
            writer.println(hashSearch.toCsvLine());
            writer.println(treeInsert.toCsvLine());
            writer.println(treeTraversal.toCsvLine());

        } catch (IOException e) {
            System.out.println("Error al generar " + OUTPUT_FILE);
            System.out.println(e.getMessage());
        }
    }

    private static class BenchmarkResult {
        private String language;
        private String operation;
        private String structure;
        private int records;
        private double timeMs;

        public BenchmarkResult(String language, String operation, String structure, int records, double timeMs) {
            this.language = language;
            this.operation = operation;
            this.structure = structure;
            this.records = records;
            this.timeMs = timeMs;
        }

        public String toCsvLine() {
            return language + ","
                    + operation + ","
                    + structure + ","
                    + records + ","
                    + format(timeMs);
        }
    }
}