using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json;
using System.Threading.Tasks;
using LMS.Models.LMSModels;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace LMS_CustomIdentity.Controllers
{
    [Authorize(Roles = "Professor")]
    public class ProfessorController : Controller
    {

        private readonly LMSContext db;

        public ProfessorController(LMSContext _db)
        {
            db = _db;
        }

        public IActionResult Index()
        {
            return View();
        }

        public IActionResult Students(string subject, string num, string season, string year)
        {
            ViewData["subject"] = subject;
            ViewData["num"] = num;
            ViewData["season"] = season;
            ViewData["year"] = year;
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

        public IActionResult Categories(string subject, string num, string season, string year)
        {
            ViewData["subject"] = subject;
            ViewData["num"] = num;
            ViewData["season"] = season;
            ViewData["year"] = year;
            return View();
        }

        public IActionResult CatAssignments(string subject, string num, string season, string year, string cat)
        {
            ViewData["subject"] = subject;
            ViewData["num"] = num;
            ViewData["season"] = season;
            ViewData["year"] = year;
            ViewData["cat"] = cat;
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

        public IActionResult Submissions(string subject, string num, string season, string year, string cat, string aname)
        {
            ViewData["subject"] = subject;
            ViewData["num"] = num;
            ViewData["season"] = season;
            ViewData["year"] = year;
            ViewData["cat"] = cat;
            ViewData["aname"] = aname;
            return View();
        }

        public IActionResult Grade(string subject, string num, string season, string year, string cat, string aname, string uid)
        {
            ViewData["subject"] = subject;
            ViewData["num"] = num;
            ViewData["season"] = season;
            ViewData["year"] = year;
            ViewData["cat"] = cat;
            ViewData["aname"] = aname;
            ViewData["uid"] = uid;
            return View();
        }

        /*******Begin code to modify********/


        /// <summary>
        /// Returns a JSON array of all the students in a class.
        /// Each object in the array should have the following fields:
        /// "fname" - first name
        /// "lname" - last name
        /// "uid" - user ID
        /// "dob" - date of birth
        /// "grade" - the student's grade in this class
        /// </summary>
        /// <param name="subject">The course subject abbreviation</param>
        /// <param name="num">The course number</param>
        /// <param name="season">The season part of the semester for the class the assignment belongs to</param>
        /// <param name="year">The year part of the semester for the class the assignment belongs to</param>
        /// <returns>The JSON array</returns>
        public IActionResult GetStudentsInClass(string subject, int num, string season, int year)
        {
            var query = from e in db.Enrolleds
                        where
                        e.ClassNavigation.Year == year &&
                        e.ClassNavigation.Season == season &&
                        e.ClassNavigation.ListingNavigation.Number == num &&
                        e.ClassNavigation.ListingNavigation.Department == subject
                        select new
                        {
                            fname = e.StudentNavigation.FName,
                            lname = e.StudentNavigation.LName,
                            uid = e.StudentNavigation.UId,
                            dob = e.StudentNavigation.Dob,
                            grade = e.Grade
                        };

            return Json(query.ToArray());
        }



        /// <summary>
        /// Returns a JSON array with all the assignments in an assignment category for a class.
        /// If the "category" parameter is null, return all assignments in the class.
        /// Each object in the array should have the following fields:
        /// "aname" - The assignment name
        /// "cname" - The assignment category name.
        /// "due" - The due DateTime
        /// "submissions" - The number of submissions to the assignment
        /// </summary>
        /// <param name="subject">The course subject abbreviation</param>
        /// <param name="num">The course number</param>
        /// <param name="season">The season part of the semester for the class the assignment belongs to</param>
        /// <param name="year">The year part of the semester for the class the assignment belongs to</param>
        /// <param name="category">The name of the assignment category in the class, 
        /// or null to return assignments from all categories</param>
        /// <returns>The JSON array</returns>
        public IActionResult GetAssignmentsInCategory(string subject, int num, string season, int year, string category)
        {
            if (category == null)
            {
                var query2 = from a in db.Assignments
                             where
                             a.CategoryNavigation.InClassNavigation.Year == year &&
                             a.CategoryNavigation.InClassNavigation.Season == season &&
                             a.CategoryNavigation.InClassNavigation.ListingNavigation.Number == num &&
                             a.CategoryNavigation.InClassNavigation.ListingNavigation.DepartmentNavigation.Subject == subject
                             select new
                             {
                                 aname = a.Name,
                                 cname = a.CategoryNavigation.Name,
                                 due = a.Due,
                                 submissions = a.Submissions.Count()
                             };

                return Json(query2.ToArray());
            }



            var query = from a in db.Assignments
                        where
                        a.CategoryNavigation.Name == category &&
                        a.CategoryNavigation.InClassNavigation.Year == year &&
                        a.CategoryNavigation.InClassNavigation.Season == season &&
                        a.CategoryNavigation.InClassNavigation.ListingNavigation.Number == num &&
                        a.CategoryNavigation.InClassNavigation.ListingNavigation.DepartmentNavigation.Subject == subject
                        select new
                        {
                            aname = a.Name,
                            cname = a.CategoryNavigation.Name,
                            due = a.Due,
                            submissions = a.Submissions.Count()
                        };

            return Json(query.ToArray());
        }


        /// <summary>
        /// Returns a JSON array of the assignment categories for a certain class.
        /// Each object in the array should have the folling fields:
        /// "name" - The category name
        /// "weight" - The category weight
        /// </summary>
        /// <param name="subject">The course subject abbreviation</param>
        /// <param name="num">The course number</param>
        /// <param name="season">The season part of the semester for the class the assignment belongs to</param>
        /// <param name="year">The year part of the semester for the class the assignment belongs to</param>
        /// <param name="category">The name of the assignment category in the class</param>
        /// <returns>The JSON array</returns>
        public IActionResult GetAssignmentCategories(string subject, int num, string season, int year)
        {
            var query = from c in db.AssignmentCategories
                         where
                         c.InClassNavigation.Year == year &&
                         c.InClassNavigation.Season == season &&
                         c.InClassNavigation.ListingNavigation.Number == num &&
                         c.InClassNavigation.ListingNavigation.DepartmentNavigation.Subject == subject
                         select new
                         {
                             name = c.Name,
                             weight = c.Weight
                         };

            return Json(query.ToArray());
        }

        /// <summary>
        /// Creates a new assignment category for the specified class.
        /// If a category of the given class with the given name already exists, return success = false.
        /// </summary>
        /// <param name="subject">The course subject abbreviation</param>
        /// <param name="num">The course number</param>
        /// <param name="season">The season part of the semester for the class the assignment belongs to</param>
        /// <param name="year">The year part of the semester for the class the assignment belongs to</param>
        /// <param name="category">The new category name</param>
        /// <param name="catweight">The new category weight</param>
        /// <returns>A JSON object containing {success = true/false} </returns>
        public IActionResult CreateAssignmentCategory(string subject, int num, string season, int year, string category, int catweight)
        {
            try
            {
                AssignmentCategory a = new AssignmentCategory();

                var inc = (from c in db.Classes where c.Year == year && c.Season == season && c.ListingNavigation.Number == num && c.ListingNavigation.DepartmentNavigation.Subject == subject select c.ClassId);

                if (inc.Count() != 1)
                    return Json(new { success = false });

                a.Weight = (uint)catweight;
                a.Name = category;
                a.InClass = inc.FirstOrDefault();


                db.AssignmentCategories.Add(a);
                db.SaveChanges();

                return Json(new { success = true });
            }
            catch (Exception e)
            {
                return Json(new { success = false });
            }

        }

        public bool UpdateStudentGrade(Enrolled e)
        {
            // assigning all grades
            var categories = (from cat in db.AssignmentCategories
                              where
                              cat.InClass == e.Class
                              select cat).ToList();

            List<double> grades = new List<double>();
            List<double> weights = new List<double>();

            double grade = 0;

            foreach (var c in categories)
            {

                double points = 0.0;
                double maxPoints = 0.0;

                var assignments = (from assignment in db.Assignments
                                   where
                                   assignment.CategoryNavigation.InClass == e.Class &&
                                   assignment.Category == c.CategoryId
                                   select assignment).ToList();

                foreach (var a in assignments)
                {

                    var sub = (from s in db.Submissions
                               where
                               s.Student == e.Student &&
                               s.Assignment == a.AssignmentId
                               select s).ToList();

                    maxPoints += a.MaxPoints;
                    points += sub.Any() ? sub.FirstOrDefault().Score : 0;

                }
                if (maxPoints == 0)
                    continue;
                grades.Add((points / maxPoints));
                weights.Add(c.Weight);
            }

            for (int i = 0; i < grades.Count; i++)
            {
                double scalingFactor = weights.Sum() / weights[i];
                grade += grades[i] * scalingFactor;
            }

            if (grade >= .93)
                e.Grade = "A";
            else if (grade >= .9)
                e.Grade = "A-";
            else if (grade >= .87)
                e.Grade = "B+";
            else if (grade >= .83)
                e.Grade = "B";
            else if (grade >= .8)
                e.Grade = "B-";
            else if (grade >= .77)
                e.Grade = "C+";
            else if (grade >= .73)
                e.Grade = "C";
            else if (grade >= .7)
                e.Grade = "C-";
            else if (grade >= .67)
                e.Grade = "D+";
            else if (grade >= .63)
                e.Grade = "D";
            else if (grade >= .6)
                e.Grade = "D-";
            else
                e.Grade = "E";


            db.SaveChanges();
            return true;
        }

        /// <summary>
        /// Creates a new assignment for the given class and category.
        /// </summary>
        /// <param name="subject">The course subject abbreviation</param>
        /// <param name="num">The course number</param>
        /// <param name="season">The season part of the semester for the class the assignment belongs to</param>
        /// <param name="year">The year part of the semester for the class the assignment belongs to</param>
        /// <param name="category">The name of the assignment category in the class</param>
        /// <param name="asgname">The new assignment name</param>
        /// <param name="asgpoints">The max point value for the new assignment</param>
        /// <param name="asgdue">The due DateTime for the new assignment</param>
        /// <param name="asgcontents">The contents of the new assignment</param>
        /// <returns>A JSON object containing success = true/false</returns>
        public IActionResult CreateAssignment(string subject, int num, string season, int year, string category, string asgname, int asgpoints, DateTime asgdue, string asgcontents)
        {
            try
            {
                // create new assignement
                Assignment a = new Assignment();

                var cat = (from c in db.AssignmentCategories
                           where
                           c.Name == category &&
                           c.InClassNavigation.Year == year &&
                           c.InClassNavigation.Season == season &&
                           c.InClassNavigation.ListingNavigation.Number == num &&
                           c.InClassNavigation.ListingNavigation.DepartmentNavigation.Subject == subject
                           select c).ToList();

                if (cat.Count() != 1)
                    return Json(new { success = false });

                a.Category = cat.FirstOrDefault().CategoryId;
                a.Name = asgname;
                a.MaxPoints = (uint)asgpoints;
                a.Due = asgdue;
                a.Contents = asgcontents;

                db.Assignments.Add(a);
                db.SaveChanges();


                var enrollds = (from e in db.Enrolleds
                                where
                                e.Class == cat.FirstOrDefault().InClass
                                select e).ToList();

                // update all student Grades
                foreach (var e in enrollds)
                {

                    UpdateStudentGrade(e);

                    db.SaveChanges();
                }
                return Json(new { success = true });
            }
            catch (Exception e)
            {
                return Json(new { success = false });
            }
        }


        /// <summary>
        /// Gets a JSON array of all the submissions to a certain assignment.
        /// Each object in the array should have the following fields:
        /// "fname" - first name
        /// "lname" - last name
        /// "uid" - user ID
        /// "time" - DateTime of the submission
        /// "score" - The score given to the submission
        /// 
        /// </summary>
        /// <param name="subject">The course subject abbreviation</param>
        /// <param name="num">The course number</param>
        /// <param name="season">The season part of the semester for the class the assignment belongs to</param>
        /// <param name="year">The year part of the semester for the class the assignment belongs to</param>
        /// <param name="category">The name of the assignment category in the class</param>
        /// <param name="asgname">The name of the assignment</param>
        /// <returns>The JSON array</returns>
        public IActionResult GetSubmissionsToAssignment(string subject, int num, string season, int year, string category, string asgname)
        {
            var query = from s in db.Submissions
                        where
                        s.AssignmentNavigation.Name == asgname &&
                        s.AssignmentNavigation.CategoryNavigation.Name == category &&
                        s.AssignmentNavigation.CategoryNavigation.InClassNavigation.Year == year &&
                        s.AssignmentNavigation.CategoryNavigation.InClassNavigation.Season == season &&
                        s.AssignmentNavigation.CategoryNavigation.InClassNavigation.ListingNavigation.Number == num &&
                        s.AssignmentNavigation.CategoryNavigation.InClassNavigation.ListingNavigation.DepartmentNavigation.Subject == subject
                        select new
                        {
                            fname = s.StudentNavigation.FName,
                            lname = s.StudentNavigation.LName,
                            uid = s.StudentNavigation.UId,
                            time = s.Time,
                            score = s.Score
                        };

            return Json(query.ToArray());
        }


        /// <summary>
        /// Set the score of an assignment submission
        /// </summary>
        /// <param name="subject">The course subject abbreviation</param>
        /// <param name="num">The course number</param>
        /// <param name="season">The season part of the semester for the class the assignment belongs to</param>
        /// <param name="year">The year part of the semester for the class the assignment belongs to</param>
        /// <param name="category">The name of the assignment category in the class</param>
        /// <param name="asgname">The name of the assignment</param>
        /// <param name="uid">The uid of the student who's submission is being graded</param>
        /// <param name="score">The new score for the submission</param>
        /// <returns>A JSON object containing success = true/false</returns>
        public IActionResult GradeSubmission(string subject, int num, string season, int year, string category, string asgname, string uid, int score)
        {
            try
            {
                // find submission
                var query = from s in db.Submissions
                            where
                            s.StudentNavigation.UId == uid &&
                            s.AssignmentNavigation.Name == asgname &&
                            s.AssignmentNavigation.CategoryNavigation.Name == category &&
                            s.AssignmentNavigation.CategoryNavigation.InClassNavigation.Year == year &&
                            s.AssignmentNavigation.CategoryNavigation.InClassNavigation.Season == season &&
                            s.AssignmentNavigation.CategoryNavigation.InClassNavigation.ListingNavigation.Number == num &&
                            s.AssignmentNavigation.CategoryNavigation.InClassNavigation.ListingNavigation.DepartmentNavigation.Subject == subject
                            select s;


                // check that it's the only submission
                if (query.Count() != 1)
                {
                    return Json(new { success = false });
                }

                // change the score
                query.FirstOrDefault().Score = (uint)score;


                // find the enrollment
                var enrollment = (from e in db.Enrolleds
                                  where
                                  e.Student == uid &&
                                  e.ClassNavigation.Year == year &&
                                  e.ClassNavigation.Season == season &&
                                  e.ClassNavigation.ListingNavigation.Number == num &&
                                  e.ClassNavigation.ListingNavigation.Department == subject
                                  select e).FirstOrDefault();

                UpdateStudentGrade(enrollment);

                return Json(new { success = true });
            }
            catch (Exception e)
            {
                return Json(new { success = false });
            }
        }


        /// <summary>
        /// Returns a JSON array of the classes taught by the specified professor
        /// Each object in the array should have the following fields:
        /// "subject" - The subject abbreviation of the class (such as "CS")
        /// "number" - The course number (such as 5530)
        /// "name" - The course name
        /// "season" - The season part of the semester in which the class is taught
        /// "year" - The year part of the semester in which the class is taught
        /// </summary>
        /// <param name="uid">The professor's uid</param>
        /// <returns>The JSON array</returns>
        public IActionResult GetMyClasses(string uid)
        {
            var query = from c in db.Classes
                        where
                        c.TaughtByNavigation.UId == uid
                        select new
                        {
                            subject = c.ListingNavigation.Department,
                            number = c.ListingNavigation.Number,
                            name = c.ListingNavigation.Name,
                            season = c.Season,
                            year = c.Year
                        };

            return Json(query.ToArray());
        }


        
        /*******End code to modify********/
    }
}

