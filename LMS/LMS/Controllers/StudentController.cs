using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using LMS.Models.LMSModels;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace LMS.Controllers
{
    [Authorize(Roles = "Student")]
    public class StudentController : Controller
    {
        private LMSContext db;
        public StudentController(LMSContext _db)
        {
            db = _db;
        }

        public IActionResult Index()
        {
            return View();
        }

        public IActionResult Catalog()
        {
            return View();
        }

        public IActionResult Class(string subject, string num, string season, string year)
        {
            ViewData["subject"] = subject;
            ViewData["num"] = num;
            ViewData["season"] = season;
            ViewData["year"] = year;
            return View();
        }

        public IActionResult Assignment(string subject, string num, string season, string year, string cat, string aname)
        {
            ViewData["subject"] = subject;
            ViewData["num"] = num;
            ViewData["season"] = season;
            ViewData["year"] = year;
            ViewData["cat"] = cat;
            ViewData["aname"] = aname;
            return View();
        }


        public IActionResult ClassListings(string subject, string num)
        {
            System.Diagnostics.Debug.WriteLine(subject + num);
            ViewData["subject"] = subject;
            ViewData["num"] = num;
            return View();
        }


        /*******Begin code to modify********/

        /// <summary>
        /// Returns a JSON array of the classes the given student is enrolled in.
        /// Each object in the array should have the following fields:
        /// "subject" - The subject abbreviation of the class (such as "CS")
        /// "number" - The course number (such as 5530)
        /// "name" - The course name
        /// "season" - The season part of the semester
        /// "year" - The year part of the semester
        /// "grade" - The grade earned in the class, or "--" if one hasn't been assigned
        /// </summary>
        /// <param name="uid">The uid of the student</param>
        /// <returns>The JSON array</returns>
        public IActionResult GetMyClasses(string uid)
        {
            var query = from e in db.Enrolleds
                        where
                        e.Student == uid
                        select new
                        {
                            subject = e.ClassNavigation.ListingNavigation.Department,
                            number = e.ClassNavigation.ListingNavigation.Number,
                            name = e.ClassNavigation.ListingNavigation.Name,
                            season = e.ClassNavigation.Season,
                            year = e.ClassNavigation.Year,
                            grade = e.Grade
                        };

            return Json(query.ToArray());
        }

        /// <summary>
        /// Returns a JSON array of all the assignments in the given class that the given student is enrolled in.
        /// Each object in the array should have the following fields:
        /// "aname" - The assignment name
        /// "cname" - The category name that the assignment belongs to
        /// "due" - The due Date/Time
        /// "score" - The score earned by the student, or null if the student has not submitted to this assignment.
        /// </summary>
        /// <param name="subject">The course subject abbreviation</param>
        /// <param name="num">The course number</param>
        /// <param name="season">The season part of the semester for the class the assignment belongs to</param>
        /// <param name="year">The year part of the semester for the class the assignment belongs to</param>
        /// <param name="uid"></param>
        /// <returns>The JSON array</returns>
        public IActionResult GetAssignmentsInClass(string subject, int num, string season, int year, string uid)
        {
            var query = from a in db.Assignments
                        where
                        a.CategoryNavigation.InClassNavigation.Year == year &&
                        a.CategoryNavigation.InClassNavigation.Season == season &&
                        a.CategoryNavigation.InClassNavigation.ListingNavigation.Number == num &&
                        a.CategoryNavigation.InClassNavigation.ListingNavigation.Department == subject
                        select new
                        {
                            aname = a.Name,
                            cname = a.CategoryNavigation.Name,
                            due = a.Due,
                            score = a.Submissions.Any() ? (uint?)a.Submissions.FirstOrDefault().Score : null
                        };

            return Json(query.ToArray());
        }



        /// <summary>
        /// Adds a submission to the given assignment for the given student
        /// The submission should use the current time as its DateTime
        /// You can get the current time with DateTime.Now
        /// The score of the submission should start as 0 until a Professor grades it
        /// If a Student submits to an assignment again, it should replace the submission contents
        /// and the submission time (the score should remain the same).
        /// </summary>
        /// <param name="subject">The course subject abbreviation</param>
        /// <param name="num">The course number</param>
        /// <param name="season">The season part of the semester for the class the assignment belongs to</param>
        /// <param name="year">The year part of the semester for the class the assignment belongs to</param>
        /// <param name="category">The name of the assignment category in the class</param>
        /// <param name="asgname">The new assignment name</param>
        /// <param name="uid">The student submitting the assignment</param>
        /// <param name="contents">The text contents of the student's submission</param>
        /// <returns>A JSON object containing {success = true/false}</returns>
        public IActionResult SubmitAssignmentText(string subject, int num, string season, int year,
          string category, string asgname, string uid, string contents)
        {
            var prevSub = from s in db.Submissions
                          where
                          s.Student == uid &&
                          s.AssignmentNavigation.Name == asgname &&
                          s.AssignmentNavigation.CategoryNavigation.Name == category &&
                          s.AssignmentNavigation.CategoryNavigation.InClassNavigation.Year == year &&
                          s.AssignmentNavigation.CategoryNavigation.InClassNavigation.Season == season &&
                          s.AssignmentNavigation.CategoryNavigation.InClassNavigation.ListingNavigation.Number == num &&
                          s.AssignmentNavigation.CategoryNavigation.InClassNavigation.ListingNavigation.Department == subject
                          select s;

            if (prevSub.Any())
            {
                prevSub.FirstOrDefault().SubmissionContents = contents;
                prevSub.FirstOrDefault().Time = DateTime.Now;
                db.SaveChanges();
                return Json(new { success = true });
            }

            try
            {
                Submission s = new Submission();

                s.Assignment = (from a in db.Assignments
                                where
                               a.Name == asgname &&
                               a.CategoryNavigation.Name == category &&
                               a.CategoryNavigation.InClassNavigation.Year == year &&
                               a.CategoryNavigation.InClassNavigation.Season == season &&
                               a.CategoryNavigation.InClassNavigation.ListingNavigation.Number == num &&
                               a.CategoryNavigation.InClassNavigation.ListingNavigation.Department == subject
                                select a.AssignmentId).FirstOrDefault();
                s.Student = uid;
                s.Score = 0;
                s.SubmissionContents = contents;
                s.Time = DateTime.Now;

                db.Submissions.Add(s);
                db.SaveChanges();

                return Json(new { success = true });

            }
            catch (Exception e)
            {
                return Json(new { success = false });
            }
        }


        /// <summary>
        /// Enrolls a student in a class.
        /// </summary>
        /// <param name="subject">The department subject abbreviation</param>
        /// <param name="num">The course number</param>
        /// <param name="season">The season part of the semester</param>
        /// <param name="year">The year part of the semester</param>
        /// <param name="uid">The uid of the student</param>
        /// <returns>A JSON object containing {success = {true/false}. 
        /// false if the student is already enrolled in the class, true otherwise.</returns>
        public IActionResult Enroll(string subject, int num, string season, int year, string uid)
        {
            var conflicts = from e in db.Enrolleds
                            where
                            e.Student == uid &&
                            e.ClassNavigation.Year == year &&
                            e.ClassNavigation.Season == season &&
                            e.ClassNavigation.ListingNavigation.Number == num &&
                            e.ClassNavigation.ListingNavigation.Department == subject
                            select e;

            if (conflicts.Any())
                return Json(new { success = false });

            try
            {
                var cl = (from c in db.Classes
                          where
                         c.Year == year &&
                         c.Season == season &&
                         c.ListingNavigation.Number == num &&
                         c.ListingNavigation.Department == subject
                          select c).FirstOrDefault();

                Enrolled e = new Enrolled();
                e.Student = uid;
                e.Class = cl.ClassId;
                e.Grade = "--";

                db.Enrolleds.Add(e);
                db.SaveChanges();

                return Json(new { success = true });
            }
            catch (Exception e)
            {
                return Json(new { success = false });
            }
        }



        /// <summary>
        /// Calculates a student's GPA
        /// A student's GPA is determined by the grade-point representation of the average grade in all their classes.
        /// Assume all classes are 4 credit hours.
        /// If a student does not have a grade in a class ("--"), that class is not counted in the average.
        /// If a student is not enrolled in any classes, they have a GPA of 0.0.
        /// Otherwise, the point-value of a letter grade is determined by the table on this page:
        /// https://advising.utah.edu/academic-standards/gpa-calculator-new.php
        /// </summary>
        /// <param name="uid">The uid of the student</param>
        /// <returns>A JSON object containing a single field called "gpa" with the number value</returns>
        public IActionResult GetGPA(string uid)
        {
            var query = from e in db.Enrolleds
                        where
                        e.Student == uid
                        select e;


            if (!query.Any())
            {
                return Json(new { gpa = 0 });
            }

            var gp = 0.0;
            foreach (var q in query)
            {
                if (q.Grade == "A")
                    gp += 4;
                else if (q.Grade == "A-")
                    gp += 3.7;
                else if (q.Grade == "B+")
                    gp += 3.3;
                else if (q.Grade == "B")
                    gp += 3;
                else if (q.Grade == "B-")
                    gp += 2.7;
                else if (q.Grade == "C+")
                    gp += 2.3;
                else if (q.Grade == "C")
                    gp += 2;
                else if (q.Grade == "C-")
                    gp += 1.7;
                else if (q.Grade == "D+")
                    gp += 1.3;
                else if (q.Grade == "D")
                    gp += 1.0;
                else if (q.Grade == "D-")
                    gp += .7;

            }

            var gpas = gp / query.Count();

            return Json(new { gpa = gpas });
        }
                
        /*******End code to modify********/

    }
}

