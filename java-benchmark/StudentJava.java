public class StudentJava {
    private int studentId;
    private String fullName;
    private String career;
    private int semester;
    private double gpa;
    private int skillScore;

    public StudentJava(int studentId, String fullName, String career, int semester, double gpa, int skillScore) {
        this.studentId = studentId;
        this.fullName = fullName;
        this.career = career;
        this.semester = semester;
        this.gpa = gpa;
        this.skillScore = skillScore;
    }

    public int getStudentId() {
        return studentId;
    }

    public String getFullName() {
        return fullName;
    }

    public String getCareer() {
        return career;
    }

    public int getSemester() {
        return semester;
    }

    public double getGpa() {
        return gpa;
    }

    public int getSkillScore() {
        return skillScore;
    }

    @Override
    public String toString() {
        return "ID: " + studentId
                + " | Nombre: " + fullName
                + " | Carrera: " + career
                + " | Semestre: " + semester
                + " | GPA: " + gpa
                + " | Skill Score: " + skillScore;
    }
}