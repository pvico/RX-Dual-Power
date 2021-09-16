#ifndef B99D9472_C491_40AD_BE50_51BB818EAF15
#define B99D9472_C491_40AD_BE50_51BB818EAF15

#include <stdint.h>
#include "initialize.h"

// #define CORRECTION_COEFF 1.013
// #define BATTERY_SENSE_PIN_RATIO (CORRECTION_COEFF * (560+110) / 110)
// #define SENSED_VOLTAGE_PER_UNIT (3.3 / 1024.0)
// #define MULTIPLY_FACTOR (SENSED_VOLTAGE_PER_UNIT * BATTERY_SENSE_PIN_RATIO)
// Multiply Factor = 0.0198840816 ±= 0.02

#define CORRECTION_VALUE -2;

initialization_result init_voltage_sensors();

// ADC frequency is 4Mhz (HSI 16MHz, prescaler /4), period 0.25µs
// ADC conversion time: 43µs
//  - Sampling time: 160.5 cycles ≂ 40µs
//  - conversion time: 12.5 cycles ≂ 3µs
// In continuous conversion with DMA, we will have multiple conversions performed
// between each 1ms loop iteration

void voltage_to_str(uint32_t voltage, uint8_t *buffer);

void voltage_sensor_loop();

void AdcGrpRegularSequenceConvComplete_Callback();
void AdcGrpRegularOverrunError_Callback();
void AdcDmaTransferComplete_Callback();
void AdcDmaTransferError_Callback();

#endif /* B99D9472_C491_40AD_BE50_51BB818EAF15 */

/*
ADC Read / Battery Voltage
0	0.00
1	0.02
2	0.04
3	0.06
4	0.08
5	0.10
6	0.12
7	0.14
8	0.16
9	0.18
10	0.20
11	0.22
12	0.24
13	0.26
14	0.28
15	0.30
16	0.32
17	0.34
18	0.36
19	0.38
20	0.40
21	0.42
22	0.44
23	0.46
24	0.48
25	0.50
26	0.52
27	0.54
28	0.56
29	0.58
30	0.60
31	0.62
32	0.64
33	0.66
34	0.68
35	0.70
36	0.72
37	0.74
38	0.76
39	0.78
40	0.80
41	0.82
42	0.84
43	0.86
44	0.87
45	0.89
46	0.91
47	0.93
48	0.95
49	0.97
50	0.99
51	1.01
52	1.03
53	1.05
54	1.07
55	1.09
56	1.11
57	1.13
58	1.15
59	1.17
60	1.19
61	1.21
62	1.23
63	1.25
64	1.27
65	1.29
66	1.31
67	1.33
68	1.35
69	1.37
70	1.39
71	1.41
72	1.43
73	1.45
74	1.47
75	1.49
76	1.51
77	1.53
78	1.55
79	1.57
80	1.59
81	1.61
82	1.63
83	1.65
84	1.67
85	1.69
86	1.71
87	1.73
88	1.75
89	1.77
90	1.79
91	1.81
92	1.83
93	1.85
94	1.87
95	1.89
96	1.91
97	1.93
98	1.95
99	1.97
100	1.99
101	2.01
102	2.03
103	2.05
104	2.07
105	2.09
106	2.11
107	2.13
108	2.15
109	2.17
110	2.19
111	2.21
112	2.23
113	2.25
114	2.27
115	2.29
116	2.31
117	2.33
118	2.35
119	2.37
120	2.39
121	2.41
122	2.43
123	2.45
124	2.47
125	2.49
126	2.51
127	2.53
128	2.55
129	2.57
130	2.58
131	2.60
132	2.62
133	2.64
134	2.66
135	2.68
136	2.70
137	2.72
138	2.74
139	2.76
140	2.78
141	2.80
142	2.82
143	2.84
144	2.86
145	2.88
146	2.90
147	2.92
148	2.94
149	2.96
150	2.98
151	3.00
152	3.02
153	3.04
154	3.06
155	3.08
156	3.10
157	3.12
158	3.14
159	3.16
160	3.18
161	3.20
162	3.22
163	3.24
164	3.26
165	3.28
166	3.30
167	3.32
168	3.34
169	3.36
170	3.38
171	3.40
172	3.42
173	3.44
174	3.46
175	3.48
176	3.50
177	3.52
178	3.54
179	3.56
180	3.58
181	3.60
182	3.62
183	3.64
184	3.66
185	3.68
186	3.70
187	3.72
188	3.74
189	3.76
190	3.78
191	3.80
192	3.82
193	3.84
194	3.86
195	3.88
196	3.90
197	3.92
198	3.94
199	3.96
200	3.98
201	4.00
202	4.02
203	4.04
204	4.06
205	4.08
206	4.10
207	4.12
208	4.14
209	4.16
210	4.18
211	4.20
212	4.22
213	4.24
214	4.26
215	4.28
216	4.29
217	4.31
218	4.33
219	4.35
220	4.37
221	4.39
222	4.41
223	4.43
224	4.45
225	4.47
226	4.49
227	4.51
228	4.53
229	4.55
230	4.57
231	4.59
232	4.61
233	4.63
234	4.65
235	4.67
236	4.69
237	4.71
238	4.73
239	4.75
240	4.77
241	4.79
242	4.81
243	4.83
244	4.85
245	4.87
246	4.89
247	4.91
248	4.93
249	4.95
250	4.97
251	4.99
252	5.01
253	5.03
254	5.05
255	5.07
256	5.09
257	5.11
258	5.13
259	5.15
260	5.17
261	5.19
262	5.21
263	5.23
264	5.25
265	5.27
266	5.29
267	5.31
268	5.33
269	5.35
270	5.37
271	5.39
272	5.41
273	5.43
274	5.45
275	5.47
276	5.49
277	5.51
278	5.53
279	5.55
280	5.57
281	5.59
282	5.61
283	5.63
284	5.65
285	5.67
286	5.69
287	5.71
288	5.73
289	5.75
290	5.77
291	5.79
292	5.81
293	5.83
294	5.85
295	5.87
296	5.89
297	5.91
298	5.93
299	5.95
300	5.97
301	5.99
302	6.00
303	6.02
304	6.04
305	6.06
306	6.08
307	6.10
308	6.12
309	6.14
310	6.16
311	6.18
312	6.20
313	6.22
314	6.24
315	6.26
316	6.28
317	6.30
318	6.32
319	6.34
320	6.36
321	6.38
322	6.40
323	6.42
324	6.44
325	6.46
326	6.48
327	6.50
328	6.52
329	6.54
330	6.56
331	6.58
332	6.60
333	6.62
334	6.64
335	6.66
336	6.68
337	6.70
338	6.72
339	6.74
340	6.76
341	6.78
342	6.80
343	6.82
344	6.84
345	6.86
346	6.88
347	6.90
348	6.92
349	6.94
350	6.96
351	6.98
352	7.00
353	7.02
354	7.04
355	7.06
356	7.08
357	7.10
358	7.12
359	7.14
360	7.16
361	7.18
362	7.20
363	7.22
364	7.24
365	7.26
366	7.28
367	7.30
368	7.32
369	7.34
370	7.36
371	7.38
372	7.40
373	7.42
374	7.44
375	7.46
376	7.48
377	7.50
378	7.52
379	7.54
380	7.56
381	7.58
382	7.60
383	7.62
384	7.64
385	7.66
386	7.68
387	7.70
388	7.72
389	7.73
390	7.75
391	7.77
392	7.79
393	7.81
394	7.83
395	7.85
396	7.87
397	7.89
398	7.91
399	7.93
400	7.95
401	7.97
402	7.99
403	8.01
404	8.03
405	8.05
406	8.07
407	8.09
408	8.11
409	8.13
410	8.15
411	8.17
412	8.19
413	8.21
414	8.23
415	8.25
416	8.27
417	8.29
418	8.31
419	8.33
420	8.35
421	8.37
422	8.39
423	8.41
424	8.43
425	8.45
426	8.47
427	8.49
428	8.51
429	8.53
430	8.55
431	8.57
432	8.59
433	8.61
434	8.63
435	8.65
436	8.67
437	8.69
438	8.71
439	8.73
440	8.75
441	8.77
442	8.79
443	8.81
444	8.83
445	8.85
446	8.87
447	8.89
448	8.91
449	8.93
450	8.95
451	8.97
452	8.99
453	9.01
454	9.03
455	9.05
456	9.07
457	9.09
458	9.11
459	9.13
460	9.15
461	9.17
462	9.19
463	9.21
464	9.23
465	9.25
466	9.27
467	9.29
468	9.31
469	9.33
470	9.35
471	9.37
472	9.39
473	9.41
474	9.43
475	9.44
476	9.46
477	9.48
478	9.50
479	9.52
480	9.54
481	9.56
482	9.58
483	9.60
484	9.62
485	9.64
486	9.66
487	9.68
488	9.70
489	9.72
490	9.74
491	9.76
492	9.78
493	9.80
494	9.82
495	9.84
496	9.86
497	9.88
498	9.90
499	9.92
500	9.94
501	9.96
502	9.98
503	10.00
504	10.02
505	10.04
506	10.06
507	10.08
508	10.10
509	10.12
510	10.14
511	10.16
512	10.18
513	10.20
514	10.22
515	10.24
516	10.26
517	10.28
518	10.30
519	10.32
520	10.34
521	10.36
522	10.38
523	10.40
524	10.42
525	10.44
526	10.46
527	10.48
528	10.50
529	10.52
530	10.54
531	10.56
532	10.58
533	10.60
534	10.62
535	10.64
536	10.66
537	10.68
538	10.70
539	10.72
540	10.74
541	10.76
542	10.78
543	10.80
544	10.82
545	10.84
546	10.86
547	10.88
548	10.90
549	10.92
550	10.94
551	10.96
552	10.98
553	11.00
554	11.02
555	11.04
556	11.06
557	11.08
558	11.10
559	11.12
560	11.14
561	11.15
562	11.17
563	11.19
564	11.21
565	11.23
566	11.25
567	11.27
568	11.29
569	11.31
570	11.33
571	11.35
572	11.37
573	11.39
574	11.41
575	11.43
576	11.45
577	11.47
578	11.49
579	11.51
580	11.53
581	11.55
582	11.57
583	11.59
584	11.61
585	11.63
586	11.65
587	11.67
588	11.69
589	11.71
590	11.73
591	11.75
592	11.77
593	11.79
594	11.81
595	11.83
596	11.85
597	11.87
598	11.89
599	11.91
600	11.93
601	11.95
602	11.97
603	11.99
604	12.01
605	12.03
606	12.05
607	12.07
608	12.09
609	12.11
610	12.13
611	12.15
612	12.17
613	12.19
614	12.21
615	12.23
616	12.25
617	12.27
618	12.29
619	12.31
620	12.33
621	12.35
622	12.37
623	12.39
624	12.41
625	12.43
626	12.45
627	12.47
628	12.49
629	12.51
630	12.53
631	12.55
632	12.57
633	12.59
634	12.61
635	12.63
636	12.65
637	12.67
638	12.69
639	12.71
640	12.73
641	12.75
642	12.77
643	12.79
644	12.81
645	12.83
646	12.85
647	12.87
648	12.88
649	12.90
650	12.92
651	12.94
652	12.96
653	12.98
654	13.00
655	13.02
656	13.04
657	13.06
658	13.08
659	13.10
660	13.12
661	13.14
662	13.16
663	13.18
664	13.20
665	13.22
666	13.24
667	13.26
668	13.28
669	13.30
670	13.32
671	13.34
672	13.36
673	13.38
674	13.40
675	13.42
676	13.44
677	13.46
678	13.48
679	13.50
680	13.52
681	13.54
682	13.56
683	13.58
684	13.60
685	13.62
686	13.64
687	13.66
688	13.68
689	13.70
690	13.72
691	13.74
692	13.76
693	13.78
694	13.80
695	13.82
696	13.84
697	13.86
698	13.88
699	13.90
700	13.92
701	13.94
702	13.96
703	13.98
704	14.00
705	14.02
706	14.04
707	14.06
708	14.08
709	14.10
710	14.12
711	14.14
712	14.16
713	14.18
714	14.20
715	14.22
716	14.24
717	14.26
718	14.28
719	14.30
720	14.32
721	14.34
722	14.36
723	14.38
724	14.40
725	14.42
726	14.44
727	14.46
728	14.48
729	14.50
730	14.52
731	14.54
732	14.56
733	14.58
734	14.59
735	14.61
736	14.63
737	14.65
738	14.67
739	14.69
740	14.71
741	14.73
742	14.75
743	14.77
744	14.79
745	14.81
746	14.83
747	14.85
748	14.87
749	14.89
750	14.91
751	14.93
752	14.95
753	14.97
754	14.99
755	15.01
756	15.03
757	15.05
758	15.07
759	15.09
760	15.11
761	15.13
762	15.15
763	15.17
764	15.19
765	15.21
766	15.23
767	15.25
768	15.27
769	15.29
770	15.31
771	15.33
772	15.35
773	15.37
774	15.39
775	15.41
776	15.43
777	15.45
778	15.47
779	15.49
780	15.51
781	15.53
782	15.55
783	15.57
784	15.59
785	15.61
786	15.63
787	15.65
788	15.67
789	15.69
790	15.71
791	15.73
792	15.75
793	15.77
794	15.79
795	15.81
796	15.83
797	15.85
798	15.87
799	15.89
800	15.91
801	15.93
802	15.95
803	15.97
804	15.99
805	16.01
806	16.03
807	16.05
808	16.07
809	16.09
810	16.11
811	16.13
812	16.15
813	16.17
814	16.19
815	16.21
816	16.23
817	16.25
818	16.27
819	16.29
820	16.30
821	16.32
822	16.34
823	16.36
824	16.38
825	16.40
826	16.42
827	16.44
828	16.46
829	16.48
830	16.50
831	16.52
832	16.54
833	16.56
834	16.58
835	16.60
836	16.62
837	16.64
838	16.66
839	16.68
840	16.70
841	16.72
842	16.74
843	16.76
844	16.78
845	16.80
846	16.82
847	16.84
848	16.86
849	16.88
850	16.90
851	16.92
852	16.94
853	16.96
854	16.98
855	17.00
856	17.02
857	17.04
858	17.06
859	17.08
860	17.10
861	17.12
862	17.14
863	17.16
864	17.18
865	17.20
866	17.22
867	17.24
868	17.26
869	17.28
870	17.30
871	17.32
872	17.34
873	17.36
874	17.38
875	17.40
876	17.42
877	17.44
878	17.46
879	17.48
880	17.50
881	17.52
882	17.54
883	17.56
884	17.58
885	17.60
886	17.62
887	17.64
888	17.66
889	17.68
890	17.70
891	17.72
892	17.74
893	17.76
894	17.78
895	17.80
896	17.82
897	17.84
898	17.86
899	17.88
900	17.90
901	17.92
902	17.94
903	17.96
904	17.98
905	18.00
906	18.01
907	18.03
908	18.05
909	18.07
910	18.09
911	18.11
912	18.13
913	18.15
914	18.17
915	18.19
916	18.21
917	18.23
918	18.25
919	18.27
920	18.29
921	18.31
922	18.33
923	18.35
924	18.37
925	18.39
926	18.41
927	18.43
928	18.45
929	18.47
930	18.49
931	18.51
932	18.53
933	18.55
934	18.57
935	18.59
936	18.61
937	18.63
938	18.65
939	18.67
940	18.69
941	18.71
942	18.73
943	18.75
944	18.77
945	18.79
946	18.81
947	18.83
948	18.85
949	18.87
950	18.89
951	18.91
952	18.93
953	18.95
954	18.97
955	18.99
956	19.01
957	19.03
958	19.05
959	19.07
960	19.09
961	19.11
962	19.13
963	19.15
964	19.17
965	19.19
966	19.21
967	19.23
968	19.25
969	19.27
970	19.29
971	19.31
972	19.33
973	19.35
974	19.37
975	19.39
976	19.41
977	19.43
978	19.45
979	19.47
980	19.49
981	19.51
982	19.53
983	19.55
984	19.57
985	19.59
986	19.61
987	19.63
988	19.65
989	19.67
990	19.69
991	19.71
992	19.73
993	19.74
994	19.76
995	19.78
996	19.80
997	19.82
998	19.84
999	19.86
1000	19.88
1001	19.90
1002	19.92
1003	19.94
1004	19.96
1005	19.98
1006	20.00
1007	20.02
1008	20.04
1009	20.06
1010	20.08
1011	20.10
1012	20.12
1013	20.14
1014	20.16
1015	20.18
1016	20.20
1017	20.22
1018	20.24
1019	20.26
1020	20.28
1021	20.30
1022	20.32
1023	20.34
*/
